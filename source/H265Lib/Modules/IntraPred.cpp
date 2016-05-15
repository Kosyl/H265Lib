#include "IntraPred.h"

namespace HEVC
{
	IntraPred::IntraPred(std::shared_ptr<SequenceParameterSet> parameters):
		sps(parameters)
	{
	}

	int IntraPred::getFilteringThreshold(size_t block_size) const
	{
		int thresh = 10;
		switch (block_size)
		{
		case 8:
			thresh = 7;
			break;
		case 16:
			thresh = 1;
			break;
		case 32:
			thresh = 0;
			break;
		};

		assert(thresh != 10); //dla block_size==4, ale dla 4 nigdy nie powinno wejsc do tej metody

		return thresh;
	}

	bool IntraPred::isFilteringRequired( ImgComp img_comp, int mode_idx, size_t block_size) const
	{
		if (img_comp != Luma)
			return false;

		if (mode_idx == IntraMode_DC || block_size == 4)
			return false;

		int dist = std::min(std::abs(mode_idx - 10), std::abs(mode_idx - 26));
		if (dist <= getFilteringThreshold(block_size))
			return false;

		return true;
	}

	Sample IntraPred::filtreSample(Sample main, Sample left, Sample right)
	{
		return (left + 2 * main + right + 2) >> 2;
	}

	void IntraPred::filtreReferenceSamples(IntraReferenceSamples &samples)
	{
		auto corner = samples.Corner;
		samples.Corner = filtreSample(corner, samples.Left[0], samples.Top[0]);

		Sample prevLeft = corner, prevTop = corner, currLeft,currTop;
		int limit = 2 * samples.block_size - 1;
		for (int x = 0; x < limit; ++x)
		{
			currLeft = samples.Left[x];
			currTop = samples.Top[x];
			samples.Left[x] = filtreSample(prevLeft, samples.Left[x], samples.Left[x + 1]);
			samples.Top[x] = filtreSample(prevTop, samples.Top[x], samples.Top[x + 1]);
			prevLeft = currLeft;
			prevTop = currTop;
		}
	}

	bool IntraPred::isSmoothingRequired(ImgComp img_comp, IntraReferenceSamples &samples) const
	{
		bool smoothPossible = (samples.block_size == 32) && sps->strong_intra_smoothing_enabled_flag && img_comp == Luma;
		if (!smoothPossible)
			return false;

		Sample limit = 1 << (sps->bit_depth_luma - 5);
		auto condLeft = std::abs(samples.Corner + samples.Left[2 * samples.block_size - 1] - 2 * samples.Left[samples.block_size - 1]);
		auto condTop = std::abs(samples.Corner + samples.Top[2 * samples.block_size - 1] - 2 * samples.Top[samples.block_size - 1]);

		bool smoothRequired = condLeft < limit && condTop < limit;
		return smoothRequired;
	}

	void IntraPred::smoothReferenceSamples(IntraReferenceSamples &samples)
	{
		assert(samples.block_size == 32);

		int limit = 2 * samples.block_size - 1;
		for (int x = 0; x < limit; ++x)
		{
			samples.Left[x] = ((63 - x) * samples.Corner + (x + 1) * samples.Left[limit] + 32) >> 6;
			samples.Top[x] = ((63 - x) * samples.Corner + (x + 1) * samples.Top[limit] + 32) >> 6;
		}
	}

	IntraMode& IntraPred::getPredictionStrategy( int mode_idx)
	{
		switch (mode_idx)
		{
		case IntraMode_Planar:
			return mode_planar;
		case IntraMode_DC:
			return mode_dc;
		case IntraMode_Horizontal:
		case IntraMode_Vertical:
			return mode_linear;
		default:
			return mode_angular;
		}
	}

	bool IntraPred::calcPuAvail(Position curr_luma_position, Position target_luma_position)
	{
		auto picWidth = sps->getPicWidth(comp);
		auto picHeight = sps->getPicHeight(comp);

		bool puOutsidePic = (target.x < 0) || (target.y < 0) || (target.x >= picWidth) || (target.y >= picHeight);

		bool puAlreadyCalc = sps->calcIdx() getZScanIdxIn4x4(neigh_x, neigh_y) < itsIdx;

		if (puOutsidePic || !puAlreadyCalc)
			return false;

		return true;
	}

	IntraReferenceSamples IntraPred::calcReference(MatrixRef<Sample> source,  std::shared_ptr<TB> tb, Picture &pic)
	{
		IntraReferenceAvailability avail(tb->size());

		bool atLeasOneAvailable = false;

		//corner
		avail.Corner = calcPuAvail(block_pos, block_pos.move(-1,-1));
		if (cornerAvailable)
		{
			itsCornerReference = itsPicRecon[X - 1][Y - 1];
			atLeasOneAvailable = true;
		}

		int offsetLimit = 2 * itsSize;

		for (int offset = 0; offset < offsetLimit; ++offset)
		{
			//left
			sideAvailable[INTRA_DIR_LEFT][offset] = calcPuAvail(X - 1, Y + offset);
			if (sideAvailable[INTRA_DIR_LEFT][offset])
			{
				itsSideReferences[INTRA_DIR_LEFT][offset] = getReferenceValue(INTRA_DIR_LEFT, offset);
				atLeasOneAvailable = true;
			}

			//up
			sideAvailable[INTRA_DIR_TOP][offset] = calcPuAvail(X + offset, Y - 1);
			if (sideAvailable[INTRA_DIR_TOP][offset])
			{
				itsSideReferences[INTRA_DIR_TOP][offset] = getReferenceValue(INTRA_DIR_TOP, offset);
				atLeasOneAvailable = true;
			}
		}

		if (!atLeasOneAvailable)
		{
			Sample def = SeqParams()->getDefaultSampleValue(itsComp);
			itsCornerReference = def;
			for (int offset = 0; offset < offsetLimit; ++offset)
			{
				itsSideReferences[INTRA_DIR_LEFT][offset] = def;
				itsSideReferences[INTRA_DIR_TOP][offset] = def;
			}
		}
		else
		{
			fillMissingReferences(sideAvailable, cornerAvailable);
		}


		delete[] sideAvailable[INTRA_DIR_LEFT];
		delete[] sideAvailable[INTRA_DIR_TOP];
		delete[] sideAvailable;

		////LOG( "OPT" ) << "Probki referencyjne:" << std::endl << itsCornerReference << " ";
		//printTable( itsSideReferences[ INTRA_DIR_TOP ], 2 * itsSize, ////LOG( "OPT" ) );

		//printMatrix( itsSideReferences, 1, 2 * itsSize, ////LOG( "OPT" ), 0, 0 );
		itsReferencesReady = true;
	}

	Matrix<Sample> IntraPred::calcPred(IntraReferenceSamples samples, ImgComp img_comp, int mode_idx)
	{
		auto size = samples.block_size;

		if (isFilteringRequired(img_comp,mode_idx, size))
		{
			if (isSmoothingRequired(img_comp,samples))
				smoothReferenceSamples(samples);
			else
				filtreReferenceSamples(samples);
		}

		IntraMode &strategy = getPredictionStrategy(mode_idx);

		return strategy.calcPred(samples, img_comp, mode_idx);
	}
}
#include <Modules/IntraMode.h>
#include <Common/Calc.h>
#include "../../IntraReferenceModel/SeqParams.h"

namespace HEVC
{
	Matrix<Sample> PlanarMode::calcPred(IntraReferenceSamples samples, ImgComp img_comp, int mode_idx)
	{
		size_t size = samples.block_size;
		int log2PuSize = Calc::log2(size);

		Matrix<Sample> pred(size);
		for (size_t x = 0; x < size; x++)
		{
			for ( size_t y = 0; y < size; y++)
			{
				int offset = (samples.Left[y] + samples.Top[x] + 1) << log2PuSize;
				int sumLeft = (x + 1) * (samples.Top[size] - samples.Left[y]);
				int sumTop = (y + 1) * (samples.Left[size] - samples.Top[x]);
				pred.at(x, y) = (offset + sumLeft + sumTop) >> (log2PuSize + 1);
				/*int x1 = ( itsReferenceValues[ INTRA_DIR_LEFT ][ y ] )*( size - 1 - x );
				int x2 = ( itsReferenceValues[ INTRA_DIR_TOP ][ x ] )*( size - 1 - y );
				int sumLeft = ( x + 1 ) * ( itsReferenceValues[ INTRA_DIR_TOP ][ size ] );
				int sumTop = ( y + 1 ) * ( itsReferenceValues[ INTRA_DIR_LEFT ][ size ] );
				pred[ x ][ y ] = ( x1 + x2 + size + sumLeft + sumTop ) >> ( log2PuSize + 1 );*/
			}
		}
		return pred;
	}

	Sample DcMode::calcDcValue(IntraReferenceSamples &samples)
	{
		auto size = samples.block_size;
		Sample dc = static_cast<Sample>(size);

		for ( size_t x = 0; x < size; ++x)
			dc += samples.Left[x] + samples.Top[x];
		dc >>= log2Int(size) + 1;

		return dc;
	}

	Matrix<Sample> DcMode::calcPred(IntraReferenceSamples samples, ImgComp img_comp, int mode_idx)
	{
		size_t blockSize = samples.block_size;
		Matrix<Sample> prediction(blockSize);

		auto dc = calcDcValue(samples);
		auto filtreEdges = (img_comp == LUMA) && (blockSize < 32);

		if (!filtreEdges)
		{
			for (auto& s : prediction)
			{
				s = dc;
			}
		}
		else
		{
			prediction(0, 0) = (samples.Left[0] + 2 * dc + samples.Top[0] + 2) >> 2;

			for ( size_t x = 1; x < blockSize; x++)
			{
				prediction(0, x) = (samples.Left[x] + 3 * dc + 2) >> 2;
				prediction(x, 0) = (samples.Top[x] + 3 * dc + 2) >> 2;
			}

			for ( size_t x = 1; x < blockSize; x++)
			{
				for ( size_t y = 1; y < blockSize; y++)
				{
					prediction(x, y) = dc;
				}
			}
		}

		return prediction;
	}

	Sample LinearMode::getFiltEdge(IntraReferenceSamples &samples, const IntraDirection dir, const ImgComp img_comp, const int offset)
	{
		auto& mainRefs = samples[dir];
		auto& sideRefs = samples.side_to(dir);
		int ref = mainRefs[0] + ((sideRefs[offset] - samples.Corner) >> 1);
		return sps->clip(img_comp, ref);
	}

	Matrix<Sample> LinearMode::calcPred(IntraReferenceSamples samples, ImgComp img_comp, int mode_idx)
	{
		assert(mode_idx == 10 || mode_idx == 26);
		assert(sps != nullptr);

		auto blockSize = samples.block_size;
		IntraDirection dir = (mode_idx == 10) ? IntraDirection::Left : IntraDirection::Top;
		auto filtreEdges = (img_comp == LUMA) && (blockSize < 32);

		Matrix<Sample> prediction(blockSize);

		if (!filtreEdges)
		{
			prediction(0, 0) = samples[dir][0];

			for ( size_t x = 1; x < blockSize; x++)
			{
				prediction(x, 0) = dir == IntraDirection::Top ? samples[IntraDirection::Top][x] : samples[IntraDirection::Left][0];
			}

			for ( size_t y = 1; y < blockSize; y++)
			{
				prediction(0, y) = dir == IntraDirection::Left ? samples[IntraDirection::Left][y] : samples[IntraDirection::Top][0];
			}
		}
		else
		{
			prediction(0, 0) = getFiltEdge(samples, dir, img_comp, 0);

			for ( size_t x = 1; x < blockSize; x++)
				prediction(x, 0) = dir == IntraDirection::Top ? samples[IntraDirection::Top][x] : getFiltEdge(samples, IntraDirection::Left, img_comp, x);

			for ( size_t y = 1; y < blockSize; y++)
				prediction(0, y) = dir == IntraDirection::Left ? samples[IntraDirection::Left][y] : getFiltEdge(samples, IntraDirection::Top, img_comp, y);
		}

		for( size_t x = 1; x < blockSize; x++ )
		{
			for( size_t y = 1; y < blockSize; y++ )
			{
				prediction( x, y ) = dir == IntraDirection::Left ? samples[ IntraDirection::Left ][ y ] : samples[ IntraDirection::Top ][ x ];
			}
		}
		return prediction;
	}

	const int AngMode::angles[] = { 32, 26, 21, 17, 13, 9, 5, 2, 0, -2, -5, -9, -13, -17, -21, -26, -32, -26, -21, -17, -13, -9, -5,
	-2, 0, 2, 5, 9, 13, 17, 21, 26, 32 };

	const int AngMode::invAngles[] = { -4096, -1638, -910, -630, -482, -390, -315, -256, -315, -390, -482, -630, -910, -1638, -4096 };

	int AngMode::getAngle(int mode_idx) const
	{
		assert(mode_idx != IntraMode_DC && mode_idx != IntraMode_Planar && mode_idx != IntraMode_Horizontal && mode_idx != IntraMode_Vertical);
		return AngMode::angles[mode_idx - 2];
	}

	int AngMode::getInvAngle(int mode_idx) const
	{
		assert((mode_idx > IntraMode_Horizontal) && (mode_idx < IntraMode_Vertical));
		return AngMode::invAngles[mode_idx - 11];
	}

	std::vector<Sample> AngMode::getRefsArray(IntraReferenceSamples &samples, int mode_idx)
	{
		int angle = getAngle(mode_idx);
		bool modeHor = mode_idx < IntraMode_18;
		size_t size = samples.block_size;

		std::vector<Sample> refsArray(3 * size + 1, 0);

		/*//LOG( "PRED" ) << "mode = " << itsCurrentPB->getModeIdx( );
		//LOG( "PRED" ) << std::endl;
		//LOG( "PRED" ) << "references: " << std::endl << itsCornerReference << " ";
		for( int i = 0; i < 2 * size; ++i )
		{
			//LOG( "PRED" ) << itsReferenceValues[ INTRA_DIR_TOP ][ i ] << " ";
		}
		//LOG( "PRED" ) << std::endl;
		for( int i = 0; i < 2 * size; ++i )
		{
			//LOG( "PRED" ) << itsReferenceValues[ INTRA_DIR_LEFT ][ i ] << std::endl;
		}*/

		int start = size;
		refsArray[start++] = samples.Corner;
		for ( size_t x = start; x <= 2 * size; x++)
		{
			refsArray[x] = modeHor ? samples.Left[x - start] : samples.Top[x - start];

			////LOG( "PRED" ) << "refsArray[ " << x << " ] <- itsReferenceValues[ " << x - start << " ] = " << refsArray[ x ] << std::endl;
		}

		if (angle < 0)
		{
			int invAngle = getInvAngle(mode_idx);
			int limit = (angle * size) >> 5;
			if (limit < -1)
			{
				for (int x = -1; x >= limit; x--)
				{
					int refIdx = ((x*invAngle + 128) >> 8) - 1;
					refsArray[size + x] = modeHor ? samples.Top[refIdx] : samples.Left[refIdx];

					////LOG( "PRED" ) << "refsArray[ " << size + x << " ] <- itsReferenceValues[ " << refIdx << " ] = " << refsArray[ size + x ] << std::ndl;
				}
			}
		}
		else
		{
			for ( size_t x = 2 * size + 1; x <= 3 * size; ++x)
			{
				refsArray[x] = modeHor ? samples.Left[x - 1 - size] : samples.Top[x - 1 - size];
				////LOG( "PRED" ) << "refsArray[ " << x << " ] <- itsReferenceValues[ " << x - 1 - size << " ] = " << refsArray[ x ] << std::endl;
			}
		}

		/*//LOG( "PRED" ) << "refs" << std::endl;
		for( int j = 0; j < 3; ++j )
		{
			for( int i = j*size; i < ( j + 1 ) * size + j / 2; ++i )
				//LOG( "PRED" ) << std::setw( 4 ) << i << " ";
			//LOG( "PRED" ) << std::endl;
			for( int i = j*size; i < ( j + 1 ) * size + j / 2; ++i )
				//LOG( "PRED" ) << std::setw( 4 ) << refsArray[ i ] << " ";
			//LOG( "PRED" ) << std::endl;
		}*/

		return refsArray;
	}

	Matrix<Sample> AngMode::calcPred(IntraReferenceSamples samples, ImgComp img_comp, int mode_idx)
	{
		auto refs = getRefsArray(samples,mode_idx);

		int angle = getAngle(mode_idx);
		bool modeHor = mode_idx < IntraMode_18;
		size_t blockSize = samples.block_size;

		Matrix<Sample> prediction(blockSize);

		int angleSum = 0, iFact, iIdx, refIdx;

		for ( size_t x = 0; x < blockSize; x++)
		{
			angleSum += angle;
			iFact = angleSum & 31;
			iIdx = angleSum >> 5;
			for ( size_t y = 0; y < blockSize; y++)
			{
				refIdx = iIdx + y + 1;
				/*//LOG( "PRED" ) << "x = " << x << ", y = " << y << std::endl;
				//LOG( "PRED" ) << "refIdx = " << refIdx << std::endl;
				//LOG( "PRED" ) << "size+refIdx = " << size + refIdx << std::endl;
				//LOG( "PRED" ) << "refs[ size+refIdx ] = " << refsArray[ size + refIdx ] << std::endl;
				//LOG( "PRED" ) << "refs[ size + refIdx + 1 ] = " << refsArray[ size + refIdx + 1 ] << std::endl;*/
				if (iFact != 0)
				{
					int comp0 = (32 - iFact) * refs[blockSize + refIdx];
					int comp1 = iFact * refs[blockSize + refIdx + 1];
					prediction(x,y) = (comp0 + comp1 + 16) >> 5;
				}
				else
				{
					prediction(x, y) = refs[blockSize + refIdx];
				}
			}
		}

		if (!modeHor)
		{
			for ( size_t y = 0; y < blockSize; y++)
			{
				for ( size_t x = y; x < blockSize; x++)
				{
					std::swap(prediction(x, y), prediction(y, x));
				}
			}
		}
		return prediction;
	}
}

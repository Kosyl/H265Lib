#ifndef INTRA_PRED_H
#define INTRA_PRED_H

#include <cassert>
#include <algorithm>

#include <Common/TypeDef.h>
#include <Common/Enums.h>
#include <Data/PU.h>
#include "IntraMode.h"
#include <Data/TB.h>

namespace HEVC
{
	class IntraPred
	{
	private:
		DcMode mode_dc;
		PlanarMode mode_planar;
		LinearMode mode_linear;
		AngMode mode_angular;
		std::shared_ptr<SequenceParameterSet> sps;

		static int getFilteringThreshold(size_t block_size);
		bool isFilteringRequired(ImgComp img_comp, int mode_idx, size_t block_size) const;
		static Sample filtreSample(Sample main, Sample left, Sample right);
		static void filtreReferenceSamples(IntraReferenceSamples &samples);

		bool isSmoothingRequired(ImgComp img_comp, IntraReferenceSamples &samples) const;
		void smoothReferenceSamples(IntraReferenceSamples &samples);

		IntraMode& getPredictionStrategy(int mode_idx);

		static bool checkSampleAvailability( Position curr_luma_position, Position target_luma_position, Picture &pic );

	public:

		IntraPred(std::shared_ptr<SequenceParameterSet> parameters);
		~IntraPred() = default;

		static IntraReferenceSamples calcReference( MatrixRef<Sample> source, TB &tb, Picture &pic );
		static void fillMissingReferences( IntraReferenceSamples &samples, IntraReferenceAvailability &availability );
		Matrix<Sample> calcPred(IntraReferenceSamples samples, ImgComp img_comp, int mode_idx );
	};
}
#endif
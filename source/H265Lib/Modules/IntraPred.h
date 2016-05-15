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

		int getFilteringThreshold(size_t block_size) const;
		bool isFilteringRequired(ImgComp img_comp, int mode_idx, size_t block_size) const;
		Sample filtreSample(Sample main, Sample left, Sample right);
		void filtreReferenceSamples(IntraReferenceSamples &samples);

		bool isSmoothingRequired(ImgComp img_comp, IntraReferenceSamples &samples) const;
		void smoothReferenceSamples(IntraReferenceSamples &samples);

		IntraMode& getPredictionStrategy(int mode_idx);

		bool calcPuAvail(const size_t targetPuX, const size_t targetPuY, Picture &pic) const;

	public:

		IntraPred(std::shared_ptr<SequenceParameterSet> parameters);
		~IntraPred() = default;

		IntraReferenceSamples calcReference(MatrixRef<Sample> source, std::shared_ptr<TB> tb, Picture &pic);
		Matrix<Sample> calcPred(IntraReferenceSamples samples, ImgComp img_comp, int mode_idx );
	};
}
#endif
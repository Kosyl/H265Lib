#pragma once

#include "BlockBase.h"


namespace HEVC
{
	struct PB : public BlockBase
	{
		ImgComp img_comp;

		MatrixRef<Sample> intra_source_samples;
		Matrix<Sample> prediction;

		int intra_mode;

		bool calcPuAvail(const int x, const int y) const;
		int getReferenceValue(const IntraDirection dir, const int offset = 0) const;

		void fillMissingReferences(bool** sideAvailable, bool cornerAvailable);

		PB(ImgComp comp, int x, int y, int size, ParametersBundle parameters);
		virtual ~PB();

		void calcAndWritePredictionToCU(std::shared_ptr<Pintra> mainPU);

		void calcReferences();

		int getPUIdx() const;

		Sample getCornerReference();

		Sample* getSideRefs(const IntraDirection);
		Sample **getPred();
		Sample **getPredForceRef(Sample* leftRefs, Sample* topRefs, Sample corner);

		virtual void print(LogId logId, bool recursive = true) override;
	};

}
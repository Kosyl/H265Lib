#pragma once

#include "BlockBase.h"


namespace HEVC
{
	class PBIntra : public BlockBase
	{
	private:

		/*TB *itsParentTB;

		ImgComp itsComp;

		Sample const* const *itsPicRecon;

		Sample** itsPredictionTarget;

		Sample** itsSideReferences;

		Sample itsCornerReference;

		bool itsReferencesReady;

		int itsModeIdx;

		bool calcPuAvail(const Int x, const Int y) const;

		int getReferenceValue(const IntraDirection dir, const int offset = 0) const;

		void fillMissingReferences(bool** sideAvailable, bool cornerAvailable);

		bool itsPredictionDone;*/

	public:

		PBIntra(ImgComp comp, int x, int y, int size, ParametersBundle parameters);

		~PBIntra();

		/*void calcAndWritePredictionToCU(std::shared_ptr<Pintra> mainPU);

		void calcReferences();

		ImgComp getImgComp() const;

		int getModeIdx() const;

		int getPUIdx() const;

		void setModeIdx(int mode);

		Sample getCornerReference();

		Sample* getSideRefs(const IntraDirection);

		Sample **getPred();

		Sample **getPredForceRef(Sample* leftRefs, Sample* topRefs, Sample corner);

		bool predictionDone()
		{
			return itsPredictionDone;
		}*/

		virtual void print(LogId logId, bool recursive = true) override;
	};

}
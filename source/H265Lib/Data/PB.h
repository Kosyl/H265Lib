#pragma once

#include "BlockBase.h"


namespace H265Lib
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

		Bool itsReferencesReady;

		UInt itsModeIdx;

		Bool calcPuAvail(const Int x, const Int y) const;

		UShort getReferenceValue(const IntraDirection dir, const UInt offset = 0) const;

		Void fillMissingReferences(Bool** sideAvailable, Bool cornerAvailable);

		Bool itsPredictionDone;*/

	public:

		PBIntra(ImgComp comp, UInt x, UInt y, UInt size, ParametersBundle parameters);

		~PBIntra();

		/*Void calcAndWritePredictionToCU(std::shared_ptr<PUIntra> mainPU);

		Void calcReferences();

		ImgComp getImgComp() const;

		UInt getModeIdx() const;

		UInt getPUIdx() const;

		Void setModeIdx(UInt mode);

		Sample getCornerReference();

		Sample* getSideRefs(const IntraDirection);

		Sample **getPred();

		Sample **getPredForceRef(Sample* leftRefs, Sample* topRefs, Sample corner);

		Bool predictionDone()
		{
			return itsPredictionDone;
		}*/

		virtual Void printDescription(Logs logId, Bool recursive = true) override;
	};

}
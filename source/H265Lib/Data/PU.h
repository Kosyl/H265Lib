#pragma once

#include <cassert>
#include "CU.h"
#include <vector>

namespace H265Lib
{
	class PUIntra;
	class TB;

	class PUIntra : public BlockBase
	{
	private:

		/*CUIntra* itsParentCU;

		UInt itsLumaModeIdx, itsChromaModeIdx;

		std::shared_ptr<TUQuadTree> itsTransformArea;*/

	public:

		PUIntra(UInt X, UInt Y, UInt size, ParametersBundle parameters);

		~PUIntra();

		/*Void setLumaModeIdx(const UInt modeIdx)
		{
			assert(modeIdx >= 0 && modeIdx <= 34);
			itsLumaModeIdx = modeIdx;
			refreshChromaModeIdx();
		}

		Void refreshChromaModeIdx()
		{
			itsChromaModeIdx = PUIntra::getModeForChroma(itsLumaModeIdx, itsParentCU->getIntraChromaPredictionDerivationType());
		}

		virtual CUIntra* getCu() const
		{
			return itsParentCU;
		}

		UInt getModeIdx(ImgComp comp) const
		{
			if (comp == LUMA)
				return itsLumaModeIdx;
			else
				return itsChromaModeIdx;
		}

		std::shared_ptr<TUQuadTree> getTransformArea() const
		{
			return itsTransformArea;
		}

		Void setTransformArea(std::shared_ptr<TUQuadTree> val)
		{
			itsTransformArea = val;
		}

		Void reconstructionLoop();

		static UInt getModeForChroma(UInt modeForLuma, UInt chromaPredictionDerivationMode);*/

		virtual Void printDescription(Logs logId) override;

		//std::vector<UInt> bestSAD8x8modes;
	};
}
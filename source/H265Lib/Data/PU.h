#pragma once

#include <cassert>
#include "CU.h"
#include <vector>

namespace HEVC
{
	class Pintra;
	class TB;

	class PU : public BlockBase
	{
	private:

		/*Cintra* itsParentCU;

		int itsLumaModeIdx, itsChromaModeIdx;

		std::shared_ptr<TUQuadTree> itsTransformArea;*/

	public:

		PU(int X, int Y, int size);

		~PU();

		/*void setLumaModeIdx(const int modeIdx)
		{
			assert(modeIdx >= 0 && modeIdx <= 34);
			itsLumaModeIdx = modeIdx;
			refreshChromaModeIdx();
		}

		void refreshChromaModeIdx()
		{
			itsChromaModeIdx = Pintra::getModeForChroma(itsLumaModeIdx, itsParentCU->getIntraChromaPredictionDerivationType());
		}

		virtual Cintra* getCu() const
		{
			return itsParentCU;
		}

		int getModeIdx(ImgComp comp) const
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

		void setTransformArea(std::shared_ptr<TUQuadTree> val)
		{
			itsTransformArea = val;
		}

		void reconstructionLoop();

		static int getModeForChroma(int modeForLuma, int chromaPredictionDerivationMode);*/

		virtual void print(LogId logId, bool recursive = true) override;

		//std::vector<int> bestSAD8x8modes;
	};
}
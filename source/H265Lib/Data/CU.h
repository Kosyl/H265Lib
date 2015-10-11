#pragma once

#include "Picture.h"
#include "BlockBase.h"

namespace H265Lib
{
	class PUIntra;
	class TUQuadTree;

	class CU : public BlockBase
	{
	protected:

		/*Sample*** itsPredictions;

		Short itsQPDeltaForCU;

		Bool itsTransQuantBypassEnabled;

		PredictionType itsPredictionType;

		Partition itsPartitionMode;

		std::shared_ptr<TUQuadTree> itsTransformTree;*/

	public:

		CU(UShort x, UShort y, UShort size, ParametersBundle parameters);

		virtual ~CU();

		/*std::shared_ptr<TUQuadTree> getTransformTree() const
		{
			return itsTransformTree;
		}

		Void setTransformTree(std::shared_ptr<TUQuadTree> val)
		{
			itsTransformTree = val;
		}

		Partition getPartitionMode() const
		{
			return itsPartitionMode;
		}

		virtual Void setPartitionMode(Partition val)
		{
			itsPartitionMode = val;
		}

		Sample** getPredictionMatrix(ImgComp comp)
		{
			return itsPredictions[comp];
		}

		PredictionType getPredictionType() const
		{
			return itsPredictionType;
		}

		Short getAbsoluteQP();

		Bool getTransQuantBypassEnabled() const
		{
			return itsTransQuantBypassEnabled;
		}

		void setTransQuantBypassEnabled(Bool val)
		{
			itsTransQuantBypassEnabled = val;
		}

		virtual Void reconstructionLoop() = 0;*/

		virtual Void printDescription( Logs logId ) override;
	};

	class CUIntra : public CU
	{
	private:

		//UShort itsIntraMPMs[3];

		//std::shared_ptr<PUIntra> itsPUs[4];

		//UShort itsChromaPredictionDerivationType;

	public:

		CUIntra(UShort x, UShort y, UShort size, ParametersBundle parameters);

		virtual ~CUIntra();

		/*UShort getIntraChromaPredictionDerivationType() const
		{
		return itsChromaPredictionDerivationType;
		}

		void setIntraChromaPredictionDerivationType(UShort val);

		virtual Void setPartitionMode(Partition val)
		{
		assert(val == Partition::Mode_2Nx2N || val == Partition::Mode_NxN);
		itsPartitionMode = val;
		for (UInt i:QT_PARTS)
		{
		if (itsPUs[i] != nullptr)
		itsPUs[i].reset();
		}
		}

		Void addPU(std::shared_ptr<PUIntra> newPU, QTComponent position = QTCOMPONENT_FIRST)
		{
		itsPUs[position] = newPU;
		}

		std::shared_ptr<PUIntra> getPU(QTComponent position = QTCOMPONENT_FIRST)
		{
		return itsPUs[position];
		}

		Void resetPUs()
		{
		for (Int i = 0; i < 4; ++i)
		itsPUs[i] = nullptr;
		}

		Void createPUs(UInt lumaModeIdx = 0);

		virtual Void reconstructionLoop();

		Double getTotalCost();
		*/

		virtual Void printDescription(Logs logId) override;
	};
}
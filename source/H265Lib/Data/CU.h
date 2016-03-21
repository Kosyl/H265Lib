#pragma once

#include "Picture.h"
#include "BlockBase.h"

namespace HEVC
{
	class Pintra;
	class TUQuadTree;

	class CU : public BlockBase
	{
	protected:

		/*Sample*** itsPredictions;

		Short itsQPDeltaForCU;

		bool itsTransQuantBypassEnabled;

		PredictionType itsPredictionType;

		Partition itsPartitionMode;

		std::shared_ptr<TUQuadTree> itsTransformTree;*/

	public:

		CU(int x, int y, int size);

		virtual ~CU();

		/*std::shared_ptr<TUQuadTree> getTransformTree() const
		{
			return itsTransformTree;
		}

		void setTransformTree(std::shared_ptr<TUQuadTree> val)
		{
			itsTransformTree = val;
		}

		Partition getPartitionMode() const
		{
			return itsPartitionMode;
		}

		virtual void setPartitionMode(Partition val)
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

		bool getTransQuantBypassEnabled() const
		{
			return itsTransQuantBypassEnabled;
		}

		void setTransQuantBypassEnabled(bool val)
		{
			itsTransQuantBypassEnabled = val;
		}

		virtual void reconstructionLoop() = 0;*/

		virtual void print(LogId logId, bool recursive = true) override;
	};

	class Cintra : public CU
	{
	private:

		//int itsIntraMPMs[3];

		//std::shared_ptr<Pintra> itsPUs[4];

		//int itsChromaPredictionDerivationType;

	public:

		Cintra(int x, int y, int size);

		virtual ~Cintra();

		/*int getIntraChromaPredictionDerivationType() const
		{
		return itsChromaPredictionDerivationType;
		}

		void setIntraChromaPredictionDerivationType(int val);

		virtual void setPartitionMode(Partition val)
		{
		assert(val == Partition::Mode_2Nx2N || val == Partition::Mode_NxN);
		itsPartitionMode = val;
		for (int i:QT_PARTS)
		{
		if (itsPUs[i] != nullptr)
		itsPUs[i].reset();
		}
		}

		void addPU(std::shared_ptr<Pintra> newPU, QTComponent position = QTCOMPONENT_FIRST)
		{
		itsPUs[position] = newPU;
		}

		std::shared_ptr<Pintra> getPU(QTComponent position = QTCOMPONENT_FIRST)
		{
		return itsPUs[position];
		}

		void resetPUs()
		{
		for (Int i = 0; i < 4; ++i)
		itsPUs[i] = nullptr;
		}

		void createPUs(int lumaModeIdx = 0);

		virtual void reconstructionLoop();

		Double getTotalCost();
		*/

		virtual void print(LogId logId, bool recursive = true) override;
	};
}
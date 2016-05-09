#include "CU.h"

namespace HEVC
{
	CU::CU(int x, int y, int size) :
		BlockBase(x, y, size)
		/*itsPartitionMode(Partition::Mode_NotSet),
		itsTransformTree(nullptr),
		itsQPDeltaForCU(0),
		itsPredictionType(PREDICTION_INTRA),
		itsTransQuantBypassEnabled(false)*/
	{
		/*itsTransformTree = std::make_shared<TUQuadTree>(this, itsX, itsY, size);
		itsPredictions = new Sample**[3];
		itsPredictions[Luma] = getEmptyMatrix<Sample>(size, size);
		itsPredictions[Cr] = getEmptyMatrix<Sample>(size / 2, size / 2);
		itsPredictions[Cb] = getEmptyMatrix<Sample>(size / 2, size / 2);*/
	}

	CU::~CU()
	{
		
	}

	void CU::setPartitionMode(Partition type)
	{
		prediction_units.clear();
		switch (type)
		{
		case Partition::Mode_NxN:
			prediction_units.push_back(std::make_shared<PU>(pos.x,							pos.y,							m_size / 2));
			prediction_units.push_back(std::make_shared<PU>(pos.x + m_size / 2, pos.y,							m_size / 2));
			prediction_units.push_back(std::make_shared<PU>(pos.x,							pos.y + m_size / 2, m_size / 2));
			prediction_units.push_back(std::make_shared<PU>(pos.x + m_size / 2, pos.y + m_size / 2, m_size / 2));
			break;
		case Partition::Mode_2Nx2N:
			prediction_units.push_back(std::make_shared<PU>(pos.x, pos.y, m_size));
			break;
		case Partition::Mode_2NxN: 
			assert(false);
			break;
		case Partition::Mode_Nx2N:
			assert(false); 
			break;
		case Partition::Mode_2NxnU:
			assert(false); 
			break;
		case Partition::Mode_2NxnD:
			assert(false);
			break;
		case Partition::Mode_nLx2N:
			assert(false); 
			break;
		case Partition::Mode_nRx2N:
			assert(false); 
			break;
		case Partition::Mode_NotSet:
			assert(false); 
			break;
		default: break;
		}
		partitioning_mode = type;
	}

	double CU::getTotalCost()
	{
		double res = 0.0;
		/*res += RDCost::getInstance()->calcCost(itsTransformTree->getTotalBins(LUMA), itsTransformTree->getTotalDistortion(LUMA), PicParams()->getQP(LUMA));
		res += RDCost::getInstance()->calcCost(itsTransformTree->getTotalBins(CB), itsTransformTree->getTotalDistortion(CB), PicParams()->getQP(CB));
		res += RDCost::getInstance()->calcCost(itsTransformTree->getTotalBins(CR), itsTransformTree->getTotalDistortion(CR), PicParams()->getQP(CR));*/
		return res;
	}

	void CU::print(LogId logId, bool recursive)
	{
		////LOG( "PART" ) << "CU[" << getX( ) << "][" << getY( ) << "], size: " << getSize( ) << std::endl;
	}
}
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

	void CU::print(LogId logId, bool recursive)
	{
		////LOG( "PART" ) << "CU[" << getX( ) << "][" << getY( ) << "], size: " << getSize( ) << std::endl;
	}

	Cintra::Cintra(int x, int y, int size):
		CU(x,y,size)
	{
	}

	/*Short CU::getAbsoluteQP()
	{
		return PicParams()->getQP(LUMA) + itsParentCTU->getParentSlice()->getQPDelta(LUMA) + itsQPDeltaForCU;
	}

	Cintra::Cintra(CTU* ctu, int x, int y, int size) :
		CU(ctu, x, y, size),
		itsChromaPredictionDerivationType(0)
	{
		for (int i = QTCOMPONENT_FIRST; i <= QTCOMPONENT_LAST; ++i)
		{
			itsPUs[i] = nullptr;
		}
		itsIntraMPMs[0] = itsIntraMPMs[1] = itsIntraMPMs[2] = 0;
		itsPredictionType = PREDICTION_INTRA;
	}*/

	Cintra::~Cintra()
	{
	}

	void Cintra::print(LogId logId, bool recursive)
	{
		////LOG( "PART" ) << "Cintra[" << getX( ) << "][" << getY( ) << "], size: " << getSize( ) << ", chromaModeDerivation: " << itsChromaPredictionDerivationType << "; partMode: " << ( itsPartitionMode == PART_NxN ? "NxN" : "2Nx2N" ) << std::endl;
		//printMatrix( itsParentPicture->getSamples( Luma ), itsSize, itsSize, //LOG( "PART" ), itsX, itsY, "" );

		/*//LOG( "PART" )  << "PUs:" << std::endl;

		//LOG_TAB );*/

		//for (QTComponent position = QTCOMPONENT_FIRST; position <= QTCOMPONENT_LAST; ++position)
		//{
		//	if (itsPUs[position] != nullptr) itsPUs[position]->printDescription();
		//}

		////LOG_UNTAB );
	}

	//void Cintra::setIntraChromaPredictionDerivationType(int val)
	//{
	//	itsChromaPredictionDerivationType = val;
	//	for (Int i = 0; i < 4; ++i)
	//	{
	//		if (itsPUs[i] != nullptr)
	//			itsPUs[i]->refreshChromaModeIdx();
	//	}
	//}

	//void Cintra::reconstructionLoop()
	//{
	//	/*//LOG( "RECO" ) << "Cintra[" << getX( ) << "][" << getY( ) << "], size: " << getSize( ) << ", chromaModeDerivation: " << itsChromaPredictionDerivationType << "; partMode: " << ( itsPartitionMode == PART_NxN ? "NxN" : "2Nx2N" ) << std::endl;
	//	//LOG_TAB "RECO" );*/

	//	if (itsPartitionMode == PART_2Nx2N)
	//	{
	//		itsPUs[0]->reconstructionLoop();
	//	}
	//	else
	//	{
	//		for (QTComponent position = QTCOMPONENT_FIRST; position <= QTCOMPONENT_LAST; ++position)
	//		{
	//			itsPUs[position]->reconstructionLoop();
	//		}
	//	}

	//	////LOG_UNTAB"RECO" );
	//}

	//void Cintra::createPUs(int lumaModeIdx)
	//{
	//	if (itsPartitionMode == PART_NxN)
	//	{
	//		for (QTComponent i = QTCOMPONENT_FIRST; i <= QTCOMPONENT_LAST; ++i)
	//		{
	//			int x = itsX + (itsSize / 2) * (i % 2);
	//			int y = itsY + (itsSize / 2) * (i / 2);
	//			std::shared_ptr<Pintra> pu = std::make_shared<Pintra>(this, x, y, itsSize / 2);
	//			pu->setLumaModeIdx(lumaModeIdx);
	//			addPU(pu, i);
	//		}
	//	}
	//	else // PART_2Nx2N
	//	{
	//		std::shared_ptr<Pintra> pu = std::make_shared<Pintra>(this, itsX, itsY, itsSize);
	//		pu->setLumaModeIdx(lumaModeIdx);
	//		addPU(pu);
	//	}
	//}

	//Double Cintra::getTotalCost()
	//{
	//	Double res = 0.0;
	//	res += RDCost::getInstance()->calcCost(itsTransformTree->getTotalBins(LUMA), itsTransformTree->getTotalDistortion(LUMA), PicParams()->getQP(LUMA));
	//	res += RDCost::getInstance()->calcCost(itsTransformTree->getTotalBins(CB), itsTransformTree->getTotalDistortion(CB), PicParams()->getQP(CB));
	//	res += RDCost::getInstance()->calcCost(itsTransformTree->getTotalBins(CR), itsTransformTree->getTotalDistortion(CR), PicParams()->getQP(CR));
	//	return res;
	//}
}
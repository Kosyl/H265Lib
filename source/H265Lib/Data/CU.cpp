#include "CU.h"

namespace H265Lib
{
	CU::CU(UShort x, UShort y, UShort size, ParametersBundle parameters) :
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

	Void CU::printDescription(LogId logId, Bool recursive)
	{
		////LOG( "PART" ) << "CU[" << getX( ) << "][" << getY( ) << "], size: " << getSize( ) << std::endl;
	}

	/*Short CU::getAbsoluteQP()
	{
		return PicParams()->getQP(LUMA) + itsParentCTU->getParentSlice()->getQPDelta(LUMA) + itsQPDeltaForCU;
	}

	CUIntra::CUIntra(CTU* ctu, UShort x, UShort y, UShort size) :
		CU(ctu, x, y, size),
		itsChromaPredictionDerivationType(0)
	{
		for (UInt i = QTCOMPONENT_FIRST; i <= QTCOMPONENT_LAST; ++i)
		{
			itsPUs[i] = nullptr;
		}
		itsIntraMPMs[0] = itsIntraMPMs[1] = itsIntraMPMs[2] = 0;
		itsPredictionType = PREDICTION_INTRA;
	}*/

	CUIntra::~CUIntra()
	{
	}

	Void CUIntra::printDescription(LogId logId, Bool recursive)
	{
		////LOG( "PART" ) << "CUIntra[" << getX( ) << "][" << getY( ) << "], size: " << getSize( ) << ", chromaModeDerivation: " << itsChromaPredictionDerivationType << "; partMode: " << ( itsPartitionMode == PART_NxN ? "NxN" : "2Nx2N" ) << std::endl;
		//printMatrix( itsParentPicture->getSamples( Luma ), itsSize, itsSize, //LOG( "PART" ), itsX, itsY, "" );

		/*//LOG( "PART" )  << "PUs:" << std::endl;

		//LOG_TAB );*/

		//for (QTComponent position = QTCOMPONENT_FIRST; position <= QTCOMPONENT_LAST; ++position)
		//{
		//	if (itsPUs[position] != nullptr) itsPUs[position]->printDescription();
		//}

		////LOG_UNTAB );
	}

	//Void CUIntra::setIntraChromaPredictionDerivationType(UShort val)
	//{
	//	itsChromaPredictionDerivationType = val;
	//	for (Int i = 0; i < 4; ++i)
	//	{
	//		if (itsPUs[i] != nullptr)
	//			itsPUs[i]->refreshChromaModeIdx();
	//	}
	//}

	//Void CUIntra::reconstructionLoop()
	//{
	//	/*//LOG( "RECO" ) << "CUIntra[" << getX( ) << "][" << getY( ) << "], size: " << getSize( ) << ", chromaModeDerivation: " << itsChromaPredictionDerivationType << "; partMode: " << ( itsPartitionMode == PART_NxN ? "NxN" : "2Nx2N" ) << std::endl;
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

	//Void CUIntra::createPUs(UInt lumaModeIdx)
	//{
	//	if (itsPartitionMode == PART_NxN)
	//	{
	//		for (QTComponent i = QTCOMPONENT_FIRST; i <= QTCOMPONENT_LAST; ++i)
	//		{
	//			UInt x = itsX + (itsSize / 2) * (i % 2);
	//			UInt y = itsY + (itsSize / 2) * (i / 2);
	//			std::shared_ptr<PUIntra> pu = std::make_shared<PUIntra>(this, x, y, itsSize / 2);
	//			pu->setLumaModeIdx(lumaModeIdx);
	//			addPU(pu, i);
	//		}
	//	}
	//	else // PART_2Nx2N
	//	{
	//		std::shared_ptr<PUIntra> pu = std::make_shared<PUIntra>(this, itsX, itsY, itsSize);
	//		pu->setLumaModeIdx(lumaModeIdx);
	//		addPU(pu);
	//	}
	//}

	//Double CUIntra::getTotalCost()
	//{
	//	Double res = 0.0;
	//	res += RDCost::getInstance()->calcCost(itsTransformTree->getTotalBins(LUMA), itsTransformTree->getTotalDistortion(LUMA), PicParams()->getQP(LUMA));
	//	res += RDCost::getInstance()->calcCost(itsTransformTree->getTotalBins(CB), itsTransformTree->getTotalDistortion(CB), PicParams()->getQP(CB));
	//	res += RDCost::getInstance()->calcCost(itsTransformTree->getTotalBins(CR), itsTransformTree->getTotalDistortion(CR), PicParams()->getQP(CR));
	//	return res;
	//}
}
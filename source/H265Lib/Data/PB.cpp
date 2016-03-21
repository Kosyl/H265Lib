#include "PB.h"

namespace HEVC
{
	PBIntra::PBIntra(ImgComp comp, int x, int y, int size, ParametersBundle parameters) :
		BlockBase( x, y, size)
		/*itsComp(comp),
		itsParentTB(parentTB),
		itsCornerReference(0),
		itsReferencesReady(false),
		itsPredictionTarget(nullptr),
		itsPredictionDone(false)*/
	{
	}

	PBIntra::~PBIntra()
	{
	}

	/*ImgComp PBIntra::getImgComp()  const
	{
		return itsComp;
	}

	int PBIntra::getPUIdx() const
	{
		return itsParentTB->getIdx();
	}

	int PBIntra::getModeIdx() const
	{
		return itsModeIdx;
	}

	void PBIntra::setModeIdx(int mode)
	{
		itsModeIdx = mode;
	}

	Sample PBIntra::getCornerReference()
	{
		if (!itsReferencesReady)
			calcReferences();

		return itsCornerReference;
	}*/

	//Sample* PBIntra::getSideRefs(const IntraDirection dir)
	//{
	//	assert(dir != INTRA_DIR_CORNER);

	//	if (!itsReferencesReady)
	//		calcReferences();

	//	return itsSideReferences[dir];
	//}

	//Sample** PBIntra::getPred()
	//{
	//	return IntraPred::getInstance()->calcPred(this);
	//}

	//Sample **PBIntra::getPredForceRef(Sample* leftRefs, Sample* topRefs, Sample corner)
	//{
	//	return IntraPred::getInstance()->calcPredForceRefs(this, leftRefs, topRefs, corner);
	//}

	//bool PBIntra::calcPuAvail(const Int targetPuX, const Int targetPuY) const
	//{
	//	Int picWidth = SeqParams()->getPicWidth() / (itsComp == LUMA ? 1 : 2);
	//	Int picHeight = SeqParams()->getPicHeight() / (itsComp == LUMA ? 1 : 2);

	//	bool puOutsidePic = (targetPuX < 0) || (targetPuY < 0) || (targetPuX >= picWidth) || (targetPuY >= picHeight);
	//	bool puAlreadyCalc = getZScanIdxIn4x4(targetPuX, targetPuY) < itsIdx;

	//	if (puOutsidePic || !puAlreadyCalc)
	//		return false;

	//	return true;
	//}

	//int PBIntra::getReferenceValue(const IntraDirection dir, const int offset) const
	//{
	//	if (dir == INTRA_DIR_LEFT)
	//		return itsPicRecon[itsX - 1][itsY + offset];
	//	else if (dir == INTRA_DIR_CORNER)
	//		return itsPicRecon[itsX - 1][itsY - 1];
	//	else
	//		return itsPicRecon[itsX + offset][itsY - 1];
	//}

	//void PBIntra::calcReferences()
	//{

	//	bool** sideAvailable = getEmptyMatrix<bool>(2, 2 * itsSize);
	//	bool cornerAvailable = false;
	//	bool atLeasOneAvailable = false;

	//	Int X = itsX;
	//	Int Y = itsY;
	//	//corner
	//	cornerAvailable = calcPuAvail(X - 1, Y - 1);
	//	if (cornerAvailable)
	//	{
	//		itsCornerReference = itsPicRecon[X - 1][Y - 1];
	//		atLeasOneAvailable = true;
	//	}

	//	int offsetLimit = 2 * itsSize;

	//	for (int offset = 0; offset < offsetLimit; ++offset)
	//	{
	//		//left
	//		sideAvailable[INTRA_DIR_LEFT][offset] = calcPuAvail(X - 1, Y + offset);
	//		if (sideAvailable[INTRA_DIR_LEFT][offset])
	//		{
	//			itsSideReferences[INTRA_DIR_LEFT][offset] = getReferenceValue(INTRA_DIR_LEFT, offset);
	//			atLeasOneAvailable = true;
	//		}

	//		//up
	//		sideAvailable[INTRA_DIR_TOP][offset] = calcPuAvail(X + offset, Y - 1);
	//		if (sideAvailable[INTRA_DIR_TOP][offset])
	//		{
	//			itsSideReferences[INTRA_DIR_TOP][offset] = getReferenceValue(INTRA_DIR_TOP, offset);
	//			atLeasOneAvailable = true;
	//		}
	//	}

	//	if (!atLeasOneAvailable)
	//	{
	//		Sample def = SeqParams()->getDefaultSampleValue(itsComp);
	//		itsCornerReference = def;
	//		for (int offset = 0; offset < offsetLimit; ++offset)
	//		{
	//			itsSideReferences[INTRA_DIR_LEFT][offset] = def;
	//			itsSideReferences[INTRA_DIR_TOP][offset] = def;
	//		}
	//	}
	//	else
	//	{
	//		fillMissingReferences(sideAvailable, cornerAvailable);
	//	}


	//	delete[] sideAvailable[INTRA_DIR_LEFT];
	//	delete[] sideAvailable[INTRA_DIR_TOP];
	//	delete[] sideAvailable;

	//	////LOG( "OPT" ) << "Probki referencyjne:" << std::endl << itsCornerReference << " ";
	//	//printTable( itsSideReferences[ INTRA_DIR_TOP ], 2 * itsSize, ////LOG( "OPT" ) );

	//	//printMatrix( itsSideReferences, 1, 2 * itsSize, ////LOG( "OPT" ), 0, 0 );
	//	itsReferencesReady = true;
	//}

	//void PBIntra::fillMissingReferences(bool** sideAvailable, bool cornerAvailable)
	//{
	//	if (sideAvailable[INTRA_DIR_LEFT][2 * itsSize - 1] == false)
	//	{
	//		for (Int idx = 2 * itsSize - 2; idx >= 0; --idx)
	//		{
	//			//scan left
	//			if (sideAvailable[INTRA_DIR_LEFT][idx])
	//			{
	//				itsSideReferences[INTRA_DIR_LEFT][2 * itsSize - 1] = itsSideReferences[INTRA_DIR_LEFT][idx];
	//				sideAvailable[INTRA_DIR_LEFT][2 * itsSize - 1] = true;
	//				break;
	//			}
	//		}

	//		if (sideAvailable[INTRA_DIR_LEFT][2 * itsSize - 1] == false && cornerAvailable)
	//		{
	//			itsSideReferences[INTRA_DIR_LEFT][2 * itsSize - 1] = itsCornerReference;
	//			sideAvailable[INTRA_DIR_LEFT][2 * itsSize - 1] = true;
	//		}

	//		if (sideAvailable[INTRA_DIR_LEFT][2 * itsSize - 1] == false)
	//		{
	//			int idxLimit = 2 * itsSize;
	//			for (int idx = 0; idx < idxLimit; ++idx)
	//			{
	//				//scan top
	//				if (sideAvailable[INTRA_DIR_TOP][idx])
	//				{
	//					itsSideReferences[INTRA_DIR_LEFT][2 * itsSize - 1] = itsSideReferences[INTRA_DIR_TOP][idx];
	//					sideAvailable[INTRA_DIR_TOP][2 * itsSize - 1] = true;
	//					break;
	//				}
	//			}
	//		}
	//	}
	//	for (Int idx = 2 * itsSize - 2; idx >= 0; --idx)
	//	{
	//		if (!sideAvailable[INTRA_DIR_LEFT][idx])
	//		{
	//			itsSideReferences[INTRA_DIR_LEFT][idx] = itsSideReferences[INTRA_DIR_LEFT][idx + 1];
	//		}
	//	}
	//	if (!cornerAvailable)
	//		itsCornerReference = itsSideReferences[INTRA_DIR_LEFT][0];
	//	if (!sideAvailable[INTRA_DIR_TOP][0])
	//		itsSideReferences[INTRA_DIR_TOP][0] = itsCornerReference;
	//	int idxLimit = 2 * itsSize;
	//	for (int idx = 1; idx < idxLimit; ++idx)
	//	{
	//		if (!sideAvailable[INTRA_DIR_TOP][idx])
	//		{
	//			itsSideReferences[INTRA_DIR_TOP][idx] = itsSideReferences[INTRA_DIR_TOP][idx - 1];
	//		}
	//	}
	//}

	//void PBIntra::calcAndWritePredictionToCU(std::shared_ptr<Pintra> mainPU)
	//{
	//	itsPredictionTarget = mainPU->getCu()->getPredictionMatrix(itsComp);
	//	itsModeIdx = mainPU->getModeIdx(itsComp);

	//	////LOG( "OPT" ) << "predykcja, comp: " << itsComp << ", tryb: " << itsModeIdx << std::endl;

	//	Sample** pred = this->getPred();
	//	int shift = (itsComp == LUMA ? 0 : 1);
	//	int offsetX = itsParentTB->getX() - (mainPU->getCu()->getX() >> shift);
	//	int offsetY = itsParentTB->getY() - (mainPU->getCu()->getY() >> shift);

	//	for (int x = offsetX; x < offsetX + itsSize; ++x)
	//	{
	//		for (int y = offsetY; y < offsetY + itsSize; ++y)
	//		{
	//			itsPredictionTarget[x][y] = pred[x - offsetX][y - offsetY];
	//		}
	//	}
	//	deleteMatrix(pred, itsSize);

	//	itsPredictionDone = true;
	//}

	void PBIntra::print(LogId logId, bool recursive)
	{
		////LOG( "OPT" ) << "PBIntra[" << getX( ) << "][" << getY( ) << "], size: " << getSize( ) << ", comp: " << itsComp << std::endl;
	}
}
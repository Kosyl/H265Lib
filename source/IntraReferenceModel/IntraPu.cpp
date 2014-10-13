#include "IntraPu.h"

IntraPu::IntraPu(const IntraCu *newCu)
{
	setCu(newCu);
}

IntraPu::IntraPu(const IntraPu &secPu)
{
	*this = secPu;
}

IntraPu& IntraPu::operator=(const IntraPu &secPu)
{
	cu = secPu.cu;
	comp = secPu.comp;
	puIdx = secPu.puIdx;
	puSize = secPu.puSize;
	modeIdx = secPu.modeIdx;
	picRecon = secPu.picRecon;
	return *this;
}

IntraPu::~IntraPu()
{
}

int IntraPu::calcPuIdx(const int puX, const int puY) const
{
	int puXDivBy4 = puX >> 2;
	int puYDivBy4 = puY >> 2;
	int calcPuIdx = ((puXDivBy4 & 8) << 3) | ((puXDivBy4 & 4) << 2) | ((puXDivBy4 & 2) << 1) | (puXDivBy4 & 1);
	calcPuIdx |= ((puYDivBy4 & 8) << 4) | ((puYDivBy4 & 4) << 3) | ((puYDivBy4 & 2) << 2) | ((puYDivBy4 & 1) << 1);
	return calcPuIdx;
}

bool IntraPu::calcPuAvail(const int puX, const int puY) const
{
	SeqParams *seqParams = SeqParams::getInstance();

	int picWidth = seqParams->getPicWidth()/(comp == LUMA ? 1 : 2);
	int picHeight = seqParams->getPicHeight()/(comp == LUMA ? 1 : 2);
	int maxCuSize = seqParams->getMaxCUSize()/(comp == LUMA ? 1 : 2);
	int cuX = cu->getCuX()/(comp == LUMA ? 1 : 2);
	int cuY = cu->getCuY()/(comp == LUMA ? 1 : 2);

	bool puOutsidePic = (puX < 0) || (puY < 0) || (puX >= picWidth) || (puY >= picHeight);
	bool puFromNextCu = ((puX - cuX >= maxCuSize) && (puY >= cuY)) || (puY - cuY >= maxCuSize);
	bool puFromPrevCu = (puX < cuX) || (puY < cuY);
	bool puAlreadyCalc = calcPuIdx(puX % maxCuSize, puY % maxCuSize) < puIdx;

	if (puOutsidePic || puFromNextCu)
		return false;
	else if (puFromPrevCu || puAlreadyCalc)
		return true;
	else
		return false;

}

int IntraPu::getReconRef(const Direction dir, const int offset) const
{
	int puX = getPuX(), puY = getPuY();
	if (dir == INTRA_DIR_LEFT)
		return picRecon[puX - 1][puY + offset];
	else if (dir == INTRA_DIR_CORNER)
		return picRecon[puX - 1][puY - 1];
	else
		return picRecon[puX + offset][puY - 1];
}

int IntraPu::getRefSubs(const Direction dir) const
{
	int puX = getPuX(), puY = getPuY();

	Direction oppDir = dir == INTRA_DIR_LEFT ? INTRA_DIR_TOP : INTRA_DIR_LEFT;
	int oppPuX = oppDir == INTRA_DIR_LEFT ? puX - 1 : puX;
	int oppPuY = oppDir == INTRA_DIR_TOP ? puY - 1 : puY;

	int refSubs = SeqParams::getInstance()->getDefVal(comp);
	if (calcPuAvail(puX - 1, puY - 1))
		refSubs = getReconRef(INTRA_DIR_CORNER);
	else if (calcPuAvail(oppPuX, oppPuY))
		refSubs = getReconRef(oppDir);
	return refSubs;
}

void IntraPu::setCu(const IntraCu *newCu)
{
	cu = newCu;
}

void IntraPu::setImgComp(const ImgComp newComp)
{
	comp = newComp;
}

void IntraPu::setPuIdx(const int newPuIdx)
{
	assert(newPuIdx >= 0);
	puIdx = newPuIdx;
}

void IntraPu::setPuSize(const int newPuSize)
{
	assert((newPuSize >= 4) && (newPuSize <= 32));
	puSize = newPuSize;
}

void IntraPu::setModeIdx(const int newModeIdx)
{
	assert((newModeIdx >= 0) && (newModeIdx <= 34));
	modeIdx = newModeIdx;
}

void IntraPu::setPicRecon(int const* const* newPicRecon)
{
	assert(picRecon != nullptr);
	picRecon = newPicRecon;
}

ImgComp IntraPu::getImgComp() const
{
	return comp;
}

int IntraPu::getPuIdx() const
{
	assert(puIdx >= 0);
	return puIdx;
}

int IntraPu::getPuSize() const
{
	assert((puSize >= 4) && (puSize <= 32));
	return puSize;
}

int IntraPu::getModeIdx() const
{
	assert((modeIdx >= 0) && (modeIdx <= 34));
	return modeIdx;
}

int IntraPu::getPuX() const
{
	int puX = ((puIdx & 64) >> 3) | ((puIdx & 16) >> 2) | ((puIdx & 4) >> 1) | (puIdx & 1);
	puX = 4 * puX + cu->getCuX()/(comp == LUMA ? 1 : 2);
	return puX;
}

int IntraPu::getPuY() const
{
	int puY = ((puIdx & 128) >> 4) | ((puIdx & 32) >> 3) | ((puIdx & 8) >> 2) | ((puIdx & 2) >> 1);
	puY = 4 * puY + cu->getCuY()/(comp == LUMA ? 1 : 2);
	return puY;
}

int IntraPu::getCorner() const
{
	int puX = getPuX(), puY = getPuY();
	int cornerRef = SeqParams::getInstance()->getDefVal(comp);
	if (calcPuAvail(puX - 1, puY - 1))
		cornerRef = getReconRef(INTRA_DIR_CORNER);
	else if (calcPuAvail(puX - 1, puY))
		cornerRef = getReconRef(INTRA_DIR_LEFT);
	else if (calcPuAvail(puX, puY - 1))
		cornerRef = getReconRef(INTRA_DIR_TOP);
	return cornerRef;
}

int* IntraPu::getSideRefs(const Direction dir) const
{
	assert(dir != INTRA_DIR_CORNER);

	int *refs = new int [2 * puSize];
	int puX = getPuX(), puY = getPuY();

	int nghPuX = dir == INTRA_DIR_LEFT ? puX - 1 : puX;
	int nghPuY = dir == INTRA_DIR_TOP ? puY - 1 : puY;
	bool neighborAvail = calcPuAvail(nghPuX, nghPuY);

	int refSubs = getRefSubs(dir);
	for (int x = 0; x < puSize; x++)
		refs[x] = neighborAvail ? getReconRef(dir, x) : refSubs;

	nghPuX = dir == INTRA_DIR_LEFT ? puX - 1 : puX + puSize;
	nghPuY = dir == INTRA_DIR_TOP ? puY - 1 : puY + puSize;
	neighborAvail = calcPuAvail(nghPuX, nghPuY);

	refSubs = refs[puSize - 1];
	for (int x = puSize; x < 2 * puSize; x++)
		refs[x] = neighborAvail ? getReconRef(dir, x) : refSubs;

	return refs;
}

int** IntraPu::getPred() const
{
	return IntraPred::getInstance()->calcPred(this);
}

int **IntraPu::getPredForceRef(const int *leftRefs, const int *topRefs, const int corner) const
{
	return IntraPred::getInstance()->calcPredForceRefs(this,leftRefs,topRefs,corner);
}


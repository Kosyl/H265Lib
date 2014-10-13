#include "IntraPu.h"
#include "IntraCu.h"

IntraCu::IntraCu(const int newCuX, const int newCuY)
{
  setCuX(newCuX);
  setCuY(newCuY);
  puList = new IntraPu* [3];
  for (int comp = 0; comp < 3; comp++)
  {
    puList[comp] = new IntraPu(this);
    resetPu(ImgComp(comp));
  }
}

IntraCu::~IntraCu()
{
  for (int comp = 0; comp < 3; comp++)
    delete puList[comp];
  delete [] puList;
}

void IntraCu::resetPu(const ImgComp comp)
{
  puList[comp]->setImgComp(comp);
  puList[comp]->setPuIdx(0);
  puList[comp]->setPuSize(4);
}

bool IntraCu::isCompCompleted(const ImgComp comp) const
{
  SeqParams *seqParams = SeqParams::getInstance();
  IntraPu *pu = puList[comp];

  int lastIdx = pow(seqParams->getMaxCUSize()/(comp == LUMA ? 4 : 8), 2);
  int maxCuSize = seqParams->getMaxCUSize()/(comp == LUMA ? 1 : 2);
  int picWidth = seqParams->getPicWidth()/(comp == LUMA ? 1 : 2);
  int picHeight = seqParams->getPicHeight()/(comp == LUMA ? 1 : 2);

  IntraPu nextPu(this);
  nextPu.setImgComp(pu->getImgComp());
  nextPu.setPuIdx(pu->getPuIdx() + pow(pu->getPuSize()/4, 2));

  bool lastPu = pu->getPuIdx() + pow(pu->getPuSize()/4, 2) == lastIdx;
  bool horFrameEnd = (nextPu.getPuX() >= picWidth) && (nextPu.getPuY() % maxCuSize == 0);
  bool verFrameEnd = (nextPu.getPuX() % maxCuSize == 0) && (nextPu.getPuY() >= picHeight);
  bool frameEnd = (pu->getPuX() >= picWidth) && (pu->getPuY() >= picHeight);

  return lastPu || horFrameEnd || verFrameEnd || frameEnd;
}

void IntraCu::setCuX(const int newCuX)
{
  assert(newCuX >= 0);
  cuX = newCuX;
}

void IntraCu::setCuY(const int newCuY)
{
  assert(newCuY >= 0);
  cuY = newCuY;
}

void IntraCu::addPu(const IntraPu& newPu)
{
  if (newPu.getImgComp() == LUMA)
    *puList[LUMA] = newPu;
  else if (newPu.getImgComp() == CB)
    *puList[CB] = newPu;
  else if (newPu.getImgComp() == CR)
    *puList[CR] = newPu;
}

int IntraCu::getCuX() const
{
  return cuX;
}

int IntraCu::getCuY() const
{
  return cuY;
}

int IntraCu::getCuWidth() const
{
  SeqParams *seqParams = SeqParams::getInstance();
  if (seqParams->getPicWidth() - cuX < seqParams->getMaxCUSize())
    return seqParams->getPicWidth() - cuX;
  else
    return seqParams->getMaxCUSize();
}

int IntraCu::getCuHeight() const
{
  SeqParams *seqParams = SeqParams::getInstance();
  if (seqParams->getPicHeight() - cuY < seqParams->getMaxCUSize())
    return seqParams->getPicHeight() - cuY;
  else
    return seqParams->getMaxCUSize();
}

bool IntraCu::isCuCompleted() const
{
  return isCompCompleted(LUMA) && isCompCompleted(CB) && isCompCompleted(CR);
}

bool IntraCu::switchToNextCu()
{
  SeqParams *seqParams = SeqParams::getInstance();

  for (int comp = 0; comp < 3; comp++)
    resetPu(ImgComp(comp));

  cuX += seqParams->getMaxCUSize();
  if (cuX >= seqParams->getPicWidth())
  {
    cuX = 0;
    cuY += seqParams->getMaxCUSize();
    if (cuY >= seqParams->getPicHeight())
    {
      cuY = 0;
      return false;
    }
  }
  return true;
}

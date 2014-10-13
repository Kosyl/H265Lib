#include "IntraPred.h"

IntraPred* IntraPred::itsInstance = nullptr;

IntraPred::IntraPred()
{
  itsModes = new IntraMode *[4];
  itsModes[PLANAR] = new PlanarMode();
  itsModes[DC] = new DcMode();
  itsModes[LINEAR] = new LinearMode();
  itsModes[ANGULAR] = new AngMode();
}

IntraPred::~IntraPred()
{
  for (int i = 0; i < 4; i++)
    delete itsModes[i];
  delete [] itsModes;
}

IntraPred* IntraPred::getInstance()
{
  if (itsInstance == nullptr)
    itsInstance = new IntraPred();
  return itsInstance;
}

int IntraPred::getFilteringThreshold() const
{
  int thresh = 10;
  switch (pu->getPuSize())
  {
    case 8:
      thresh = 7;
      break;
    case 16:
      thresh = 1;
      break;
    case 32:
      thresh = 0;
      break;
  };
  return thresh;
}

bool IntraPred::isFilteringRequired() const
{
  bool skipFiltration = (pu->getImgComp() != LUMA) || (pu->getModeIdx() == 1);
  int dist = std::min(abs(pu->getModeIdx() - 10), abs(pu->getModeIdx() - 26));
  bool distTooSmall = dist <= getFilteringThreshold();
  if (skipFiltration | distTooSmall)
    return false;
  return true;
}

int IntraPred::filtRef(const int mainRef, const int leftRef, const int rightRef) const
{
  return (leftRef + 2 * mainRef + rightRef + 2) >> 2;
}

void IntraPred::filterSideRefs(const Direction dir)
{
  int *refs = dir == INTRA_DIR_LEFT ? itsLeftRefs : itsTopRefs;

  int prevRef = itsCornerValue, currRef;
  for (int x = 0; x < 2 * pu->getPuSize() - 1; x++, prevRef = currRef)
  {
    currRef = refs[x];
    refs[x] = filtRef(refs[x], prevRef, refs[x + 1]);
  }
}

void IntraPred::filter()
{
  assert((itsLeftRefs != nullptr) && (itsTopRefs != nullptr));

  int firstLeft = itsLeftRefs[0];
  int firstTop = itsTopRefs[0];

  filterSideRefs(INTRA_DIR_LEFT);
  filterSideRefs(INTRA_DIR_TOP);

  itsCornerValue = filtRef(itsCornerValue, firstLeft, firstTop);
}

bool IntraPred::checkSmoothCond(const Direction dir) const
{
  assert(dir != INTRA_DIR_CORNER);
  const int *currRefs = dir == INTRA_DIR_LEFT ? itsLeftRefs : itsTopRefs;
  int cond = abs(itsCornerValue + currRefs[2 * pu->getPuSize() - 1] - 2 * currRefs[pu->getPuSize() - 1]);
  int limit = 1 << (SeqParams::getInstance()->getBitDepthLuma() - 5);
  return cond < limit;
} 

bool IntraPred::isSmoothReq() const
{
  bool skipFiltration = (pu->getImgComp() != LUMA) || (pu->getModeIdx() == 1);
  bool skipSmoothing = (pu->getPuSize() != 32) || !SeqParams::getInstance()->getSmoothEn();
  bool smoothCond = checkSmoothCond(INTRA_DIR_LEFT) && checkSmoothCond(INTRA_DIR_TOP);
  return !(skipFiltration || skipSmoothing) && smoothCond;
}

int IntraPred::getSmoothedReferenceAtPosition(const Direction dir, const int offset) const
{
  int lastRef = dir == INTRA_DIR_LEFT ? itsLeftRefs[2 * pu->getPuSize() - 1] : itsTopRefs[2 * pu->getPuSize() - 1];
  return ((63 - offset) * itsCornerValue + (offset + 1) * lastRef + 32) >> 6;
}

void IntraPred::smoothSideRefs(const Direction dir)
{
  int *refs = dir == INTRA_DIR_LEFT ? itsLeftRefs : itsTopRefs;
  for (int x = 0; x < 2 * pu->getPuSize(); x++)
    refs[x] = getSmoothedReferenceAtPosition(dir, x);
}

void IntraPred::doReferenceSmoothing()
{
  smoothSideRefs(INTRA_DIR_LEFT);
  smoothSideRefs(INTRA_DIR_TOP);
}

IntraMode *IntraPred::getPredictionStrategy()
{
  switch (pu->getModeIdx())
  {
    case 0:
      return itsModes[PLANAR];
    case 1:
      return itsModes[DC];
    case 10:
    case 26:
      return itsModes[LINEAR];
    default:
      return itsModes[ANGULAR];
  }
}

int **IntraPred::calcPred(const IntraPu *newPu)
{
  assert(newPu != nullptr);

  pu = newPu;

  itsCornerValue = pu->getCorner();
  itsLeftRefs = pu->getSideRefs(INTRA_DIR_LEFT);
  itsTopRefs = pu->getSideRefs(INTRA_DIR_TOP);

  if (isFilteringRequired())
  {
    if (isSmoothReq())
      doReferenceSmoothing();
    else 
      filter();
  }

  IntraMode *strategy = getPredictionStrategy();
  strategy->setPu(pu);
  strategy->setCorner(itsCornerValue);
  strategy->setSideRefs(INTRA_DIR_LEFT, itsLeftRefs);
  strategy->setSideRefs(INTRA_DIR_TOP, itsTopRefs);

  int **pred = strategy->calcPred();

  delete [] itsLeftRefs;
  delete [] itsTopRefs;

  return pred;
}

int **IntraPred::calcPredForceRefs(const IntraPu *newPu, const int *leftRefs, const int *topRefs, const int corner)
{
  assert(newPu != nullptr);

  pu = newPu;

  if (isFilteringRequired())
  {
    if (isSmoothReq())
      doReferenceSmoothing();
    else 
      filter();
  }

  IntraMode *strategy = getPredictionStrategy();
  strategy->setPu(pu);
  strategy->setCorner(corner);
  strategy->setSideRefs(INTRA_DIR_LEFT, leftRefs);
  strategy->setSideRefs(INTRA_DIR_TOP, topRefs);

  int **pred = strategy->calcPred();

  return pred;
}
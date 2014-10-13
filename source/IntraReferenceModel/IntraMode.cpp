#include "IntraPu.h"
#include "IntraMode.h"

IntraMode::IntraMode()
{
  pu = nullptr;
  leftRefs = topRefs = nullptr;
}

IntraMode::~IntraMode()
{
}

int **IntraMode::initPred() const
{
  int **pred = new int *[pu->getPuSize()];
  for (int x = 0; x < pu->getPuSize(); x++)
    pred[x] = new int[pu->getPuSize()];
  return pred;
}

void IntraMode::setPu(const IntraPu *newPu)
{
  pu = newPu;
}

void IntraMode::setCorner(const int newCorner)
{
  itsCornerReference = newCorner;
}

void IntraMode::setSideRefs(const Direction dir, const int *refs)
{
  assert(dir != INTRA_DIR_CORNER);

  if (dir == INTRA_DIR_LEFT)
    leftRefs = refs;
  else
    topRefs = refs;
}

PlanarMode::PlanarMode()
{
}

PlanarMode::~PlanarMode()
{
}

int **PlanarMode::calcPred()
{
  assert(pu != nullptr);
  assert((leftRefs != nullptr) || (topRefs != nullptr));

  int log2PuSize = log2Int(pu->getPuSize());

  int **pred = initPred();
  for (int x = 0; x < pu->getPuSize(); x++)
  {
    for (int y = 0; y < pu->getPuSize(); y++)
    {
      int offset = (leftRefs[y] + topRefs[x] + 1) << log2PuSize;
      int sumLeft = (x + 1) * (topRefs[pu->getPuSize()] - leftRefs[y]);
      int sumTop = (y + 1) * (leftRefs[pu->getPuSize()] - topRefs[x]);
      pred[x][y] = (offset + sumLeft + sumTop) >> (log2PuSize + 1);
    }
  }
  return pred;
}

DcMode::DcMode()
{
}

DcMode::~DcMode()
{
}

void DcMode::calcDcVal()
{
  dcVal = pu->getPuSize();
  for (int x = 0; x < pu->getPuSize(); x++)
    dcVal += leftRefs[x] + topRefs[x];
  dcVal >>= log2Int(pu->getPuSize()) + 1;
}

int DcMode::getFiltCorner() const
{
  bool filtCorner = (pu->getImgComp() == LUMA) && (pu->getPuSize() < 32);
  if (filtCorner)
    return (leftRefs[0] + 2 * dcVal + topRefs[0] + 2) >> 2;
  else
    return dcVal;
}

int DcMode::getFiltEdge(const Direction dir, const int offset) const
{
  assert(dir != INTRA_DIR_CORNER);
  assert((offset >= 0) && (offset < pu->getPuSize()));

  const int *refs = dir == INTRA_DIR_LEFT ? leftRefs : topRefs;
  bool filtRef = (pu->getImgComp() == LUMA) && (pu->getPuSize() < 32);

  if (filtRef)
    return (refs[offset] + 3 * dcVal + 2) >> 2;
  else
    return dcVal;
}

int **DcMode::calcPred()
{
  assert(pu != nullptr);
  assert((leftRefs != nullptr) || (topRefs != nullptr));

  calcDcVal();

  int **pred = initPred();
  pred[0][0] = getFiltCorner();
  for (int x = 1; x < pu->getPuSize(); x++)
  {
    pred[0][x] = getFiltEdge(INTRA_DIR_LEFT, x);
    pred[x][0] = getFiltEdge(INTRA_DIR_TOP, x);
  }

  for (int x = 1; x < pu->getPuSize(); x++)
    for (int y = 1; y < pu->getPuSize(); y++)
      pred[x][y] = dcVal;

  return pred;
}

LinearMode::LinearMode()
{
}

LinearMode::~LinearMode()
{
}

int LinearMode::getFiltEdge(const Direction dir, const int offset)
{
  const int *mainRefs = dir == INTRA_DIR_LEFT ? leftRefs : topRefs;
  const int *sideRefs = dir == INTRA_DIR_LEFT ? topRefs : leftRefs;
  int ref = mainRefs[0] + ((sideRefs[offset] - itsCornerReference) >> 1);
  return SeqParams::getInstance()->clip(pu->getImgComp(), ref);
}

int **LinearMode::calcPred()
{
  assert(pu != nullptr);
  assert((leftRefs != nullptr) || (topRefs != nullptr));

  Direction dir = pu->getModeIdx() == 10 ? INTRA_DIR_LEFT : INTRA_DIR_TOP;
  bool filtEdge = (pu->getImgComp() == LUMA) && (pu->getPuSize() < 32);

  int **pred = initPred();

  pred[0][0] = dir == INTRA_DIR_LEFT ? leftRefs[0] : topRefs[0];
  if (filtEdge)
    pred[0][0] = getFiltEdge(dir, 0);

  for (int x = 1; x < pu->getPuSize(); x++)
    pred[x][0] = dir == INTRA_DIR_TOP ? topRefs[x] : filtEdge ? getFiltEdge(INTRA_DIR_LEFT, x) : leftRefs[0];

  for (int y = 1; y < pu->getPuSize(); y++)
    pred[0][y] = dir == INTRA_DIR_LEFT ? leftRefs[y] : filtEdge ? getFiltEdge(INTRA_DIR_TOP, y) : topRefs[0];

  for (int x = 1; x < pu->getPuSize(); x++)
    for (int y = 1; y < pu->getPuSize(); y++)
      pred[x][y] = dir == INTRA_DIR_LEFT ? leftRefs[y] : topRefs[x];

  return pred;
}

const int AngMode::angles[] = {32, 26, 21, 17, 13, 9, 5, 2, 0, -2, -5, -9, -13, -17, -21, -26, -32, -26, -21, -17, -13, -9, -5,
                              -2, 0, 2, 5, 9, 13, 17, 21, 26, 32};

const int AngMode::invAngles[] = {4096, 1638, 910, 630, 482, 390, 315, 256, 315, 390, 482, 630, 910, 1638, 4096};

AngMode::AngMode()
{
  refsArray = nullptr;
}

AngMode::~AngMode()
{
  if (refsArray != nullptr)
    delete [] refsArray;
}

int AngMode::getAngle() const
{
  assert((pu->getModeIdx() != 0) && (pu->getModeIdx() != 1) && (pu->getModeIdx() != 10) && (pu->getModeIdx() != 26));
  return AngMode::angles[pu->getModeIdx() - 2];
}

int AngMode::getInvAngle() const
{
  assert((pu->getModeIdx() > 10) || (pu->getModeIdx() < 26));
  return AngMode::invAngles[pu->getModeIdx() - 11];
}

void AngMode::getRefsArray()
{
  assert(pu != nullptr);
  assert((leftRefs != nullptr) || (topRefs != nullptr));

  int angle = getAngle();

  bool modeHor = pu->getModeIdx() < 18;

  if (refsArray != nullptr)
    delete [] refsArray;
  refsArray = new int [2 * pu->getPuSize() + 1];

  int start = angle > 0 ? 0 : pu->getPuSize();
  refsArray[start++] = itsCornerReference;
  for (int x = start; x < 2 * pu->getPuSize() + 1; x++)
    refsArray[x] = modeHor ? leftRefs[x - start] : topRefs[x - start];

  if (angle < 0)
  {
    int invAngle = getInvAngle();
    int invAngleSum = 128;
    int limit = (angle * pu->getPuSize()) >> 5;
    for (int x = -1; x > limit; x--)
    {
      invAngleSum += invAngle;
      int refIdx = (invAngleSum >> 8) - 1;
      refsArray[pu->getPuSize() + x] = modeHor ? topRefs[refIdx] : leftRefs[refIdx];
    }
  }
}

int **AngMode::calcPred()
{
  assert(pu != nullptr);
  assert((leftRefs != nullptr) || (topRefs != nullptr));

  getRefsArray();

  int angle = getAngle();
  int *refs = angle >= 0 ? refsArray : refsArray + pu->getPuSize();

  bool modeHor = pu->getModeIdx() < 18;

  int **pred = initPred();
  int angleSum = 0, iFact, iIdx, refIdx;
  for (int x = 0; x < pu->getPuSize(); x++)
  {
    angleSum += angle;
    iFact = angleSum & 31;
    iIdx = angleSum >> 5;
    for (int y = 0; y < pu->getPuSize(); y++)
    {
      refIdx = iIdx + y + 1;
      int comp0 = iFact != 32 ? (32 - iFact) * refs[refIdx] : 0;
      int comp1 = iFact != 0 ? iFact * refs[refIdx + 1] : 0;
      pred[x][y] = (comp0 + comp1 + 16) >> 5;
    }
  }

  if (!modeHor)
    for (int y = 0; y < pu->getPuSize(); y++)
      for (int x = y; x < pu->getPuSize(); x++)
      {
        int swap = pred[x][y];
        pred[x][y] = pred[y][x];
        pred[y][x] = swap;
      }

  delete [] refsArray;
  refsArray = nullptr;

  return pred;
}

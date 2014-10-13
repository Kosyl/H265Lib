#include "SeqParams.h"

SeqParams* SeqParams::instance = nullptr;

SeqParams::SeqParams()
{
  picWidth = picHeight = bitDepthLuma = bitDepthChroma = maxCuSize = -1;
  smoothEn = false;
}
 
SeqParams::~SeqParams()
{
}

SeqParams* SeqParams::getInstance()
{
  if (instance == nullptr)
    instance = new SeqParams();
  return instance;
}

void SeqParams::setPicWidth(const int newPicWidth)
{
  picWidth = newPicWidth;
}

void SeqParams::setPicHeight(const int newPicHeight)
{
  picHeight = newPicHeight;
}

void SeqParams::setBitDepthLuma(const int newBitDepthLuma)
{
  bitDepthLuma = newBitDepthLuma;
}

void SeqParams::setBitDepthChroma(const int newBitDepthChroma)
{
  bitDepthChroma = newBitDepthChroma;
}

void SeqParams::setMaxCUSize(const int newMaxCuSize)
{
  maxCuSize = newMaxCuSize;
}

void SeqParams::setSmoothEn(const bool newSmoothEn)
{
  smoothEn = newSmoothEn;
}

int SeqParams::getPicWidth() const
{
  return picWidth;
}

int SeqParams::getPicHeight() const
{
  return picHeight;
}

int SeqParams::getBitDepthLuma() const
{
  return bitDepthLuma;
}


int SeqParams::getBitDepthChroma() const
{
  return bitDepthChroma;
}

int SeqParams::getMaxCUSize() const
{
  return maxCuSize;
}

bool SeqParams::getSmoothEn() const
{
  return smoothEn;
}

int SeqParams::clip(ImgComp comp, int ref) const
{
  int maxVal = comp == LUMA ? 1 << bitDepthLuma : 1 << bitDepthChroma;
  if (ref < 0)
    return 0;
  else if (ref > maxVal)
    return maxVal;
  else
    return ref;
}

int SeqParams::getDefVal(ImgComp comp) const
{
  if (comp == LUMA)
    return 1 << (bitDepthLuma - 1);
  else
    return 1 << (bitDepthChroma - 1);
}


int log2Int(const int value)
{
  assert(value > 0);

  int log2 = 0, val = value;
  for (; val != 1; val >>= 1, log2++);
  return log2;
}

ImgComp getImgComp(const std::string line)
{
  if (line == "Cb")
    return CB;
  else if (line == "Cr")
    return CR;
  else
    return LUMA;
}

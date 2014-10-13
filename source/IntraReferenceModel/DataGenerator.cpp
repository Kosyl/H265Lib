#include "DataGenerator.h"

PredParams *PredParams::lumaInstance = nullptr;
PredParams *PredParams::chromaInstance = nullptr;

PredParams::PredParams()
{
  posIdx = 0;
  lumaModes[0] = lumaModes[1] = lumaModes[2] = -1;
}

PredParams *PredParams::getLumaInstance()
{
  if (lumaInstance == nullptr)
    lumaInstance = new PredParams();
  return lumaInstance;
}

PredParams *PredParams::getChromaInstance()
{
  if (chromaInstance == nullptr)
    chromaInstance = new PredParams();
  return chromaInstance;
}

int PredParams::getLumaMode(const int puSize)
{
  PredParams *predParams = PredParams::getLumaInstance();
  int puSizeIdx = log2Int(puSize) - 2;
  int lumaMode = predParams->lumaModes[puSizeIdx];
  if (lumaMode == -1)
    lumaMode = predParams->selectLumaMode(puSize);
  predParams->lumaModes[puSizeIdx] = lumaMode;
  return lumaMode;
}

int PredParams::selectLumaMode(const int puSize)
{
  std::vector<int> posModes;
  for (int i = 0; i < predPos.size(); i++)
  {
    bool correctPuSize = (predPos[i].puSize == puSize/2);
    if (puSize == 4)
      correctPuSize = correctPuSize || ((predPos[i].puSize == 4) && (predPos[i].pu4x4Idx == 0));

    if (correctPuSize)
      posModes.push_back(predPos[i].modeIdx);
  }

  srand (time(NULL));
  int selModeIdx = rand() % posModes.size();
  return posModes[selModeIdx];
}

void PredParams::addPos(const int puSize, const int modeIdx, const int pu4x4Idx)
{ 
  assert((puSize >= 0) && (puSize <= 32));
  assert((modeIdx >= 0) && (modeIdx <= 35));
  assert((pu4x4Idx >= 0) && (pu4x4Idx <= 3));
   
  PredPosition newPos = {puSize, modeIdx, pu4x4Idx};
  predPos.push_back(newPos);
}

void PredParams::switchToNextPos()
{
  posIdx++;
  if (posIdx >= predPos.size())
    posIdx = 0;
}

bool PredParams::firstPosition() const
{
  return posIdx == 0 ? true : false;
}

int PredParams::getPuSize() const
{
  assert(predPos.size() != 0);
  return predPos[posIdx].puSize;
}

int PredParams::getModeIdx() const
{
  assert(predPos.size() != 0);
  return predPos[posIdx].modeIdx;
}

int PredParams::getPu4x4Idx() const
{
  assert(predPos.size() != 0);
  return predPos[posIdx].pu4x4Idx;
}

PredWriter::PredWriter(std::ofstream *newPredFile, std::ofstream *newReconFile, IntraCu *cu, int **recon)
{
  assert((newPredFile != nullptr) && (newReconFile != nullptr) && (recon != nullptr));
  predFile = newPredFile;
  reconFile = newReconFile;
  blockIdx = 0;
  pu = new IntraPu(cu);
  pu->setPicRecon(recon);
  picRecon = recon;
}

PredWriter::~PredWriter()
{
  delete pu;
}

int PredWriter::getBlockX() const
{
  int blockX = ((blockIdx & 64) >> 3) | ((blockIdx & 16) >> 2) | ((blockIdx & 4) >> 1) | (blockIdx & 1);
  blockX = 8 * blockX/(pu->getImgComp() == LUMA ? 1 : 2);
  return blockX;
}

int PredWriter::getBlockY() const
{
  int blockY = ((blockIdx & 128) >> 4) | ((blockIdx & 32) >> 3) | ((blockIdx & 8) >> 2) | ((blockIdx & 2) >> 1);
  blockY = 8 * blockY/(pu->getImgComp() == LUMA ? 1 : 2);
  return blockY;
}

std::string PredWriter::getImgComp() const
{
  if (pu->getImgComp() == CB)
    return "CB";
  else if (pu->getImgComp() == CR)
    return "CR";
  else
    return "LUMA";
}

void PredWriter::storeHeader(std::ofstream *targetFile) const
{
  *targetFile << "############" << std::endl;
  *targetFile << "Block Index         : " << blockIdx << std::endl;
  *targetFile << "Component           : " << (int) pu->getImgComp() << std::endl;
  *targetFile << "PU Index            : " << pu->getPuIdx() << std::endl;
  *targetFile << "PU Size             : " << pu->getPuSize() << std::endl;
  *targetFile << "Mode Idx            : " << pu->getModeIdx() << std::endl;
  *targetFile << "############" << std::endl;
}

void PredWriter::storePredSamples() const
{
  int blockX = getBlockX() % pu->getPuSize();
  int blockY = getBlockY() % pu->getPuSize();
  int blockSize = pu->getPuSize() > 4 ? 8 : 4;

/*  if (pu->getImgComp() != LUMA)
  {
    blockX = 0;
    blockY = 0;
    blockSize = pu->getPuSize();
  } */

  int **source = pu->getPred();
  for (int y = blockY; y < blockY + blockSize; y++)
  {
    for (int x = blockX; x < blockX + blockSize; x++)
      *predFile << source[x][y] << " ";
    *predFile << "\n";
  }
  *predFile << "############" << std::endl;

  for (int x = 0; x < pu->getPuSize(); x++)
    delete [] source[x];
  delete [] source;
}

void PredWriter::storeReconSamples() const
{
  for (int y = pu->getPuY(); y < pu->getPuY() + pu->getPuSize(); y++)
  {
    for (int x = pu->getPuX(); x < pu->getPuX() + pu->getPuSize(); x++)
      *reconFile << picRecon[x][y] << " ";
    *reconFile << std::endl;
  }
}

void PredWriter::switchToNextBlock()
{
  SeqParams *seqParams = SeqParams::getInstance();
  int maxBlockIdx = (int)pow(seqParams->getMaxCUSize()/8, 2);
  blockIdx++;
  if (blockIdx == maxBlockIdx)
    blockIdx = 0;
}

bool PredWriter::firstBlock() const
{
  return blockIdx == 0 ? true : false;
}

LumaPredWriter::LumaPredWriter(std::ofstream *newPredFile, std::ofstream *newReconFile, IntraCu *cu,
                               int **recon) : PredWriter(newPredFile, newReconFile, cu, recon)
{
  pu->setImgComp(LUMA);
}

LumaPredWriter::~LumaPredWriter()
{
}

void LumaPredWriter::setPuIdx(const int pu4x4Idx)
{
  int puSizeStep = (int)pow(pu->getPuSize()/4, 2);
  int blockIdxNorm = (4 * blockIdx) / puSizeStep;
  int puIdx = puSizeStep * blockIdxNorm;
  puIdx = pu->getPuSize() == 4 ? puIdx + pu4x4Idx : puIdx;
  pu->setPuIdx(puIdx);
}

void LumaPredWriter::updatePu()
{
  PredParams *predParams = PredParams::getLumaInstance();
  pu->setPuSize(predParams->getPuSize());
  pu->setModeIdx(predParams->getModeIdx());
  setPuIdx(predParams->getPu4x4Idx());
}

void LumaPredWriter::storeBlock()
{
  PredParams *predParams = PredParams::getLumaInstance();
  bool puSizes[] = {false, false, false, false};

  bool contChecking = true;
  while (contChecking)
  {
    updatePu();
    storeHeader(predFile);
    storePredSamples();
    puSizes[log2Int(pu->getPuSize()) - 2] = true;
    predParams->switchToNextPos();
    contChecking = !predParams->firstPosition();
  }

  if (puSizes[0])
  {
    pu->setPuSize(4);
    for (int pu4x4Idx = 0; pu4x4Idx < 4; pu4x4Idx++)
    {
      setPuIdx(pu4x4Idx);
      storeHeader(reconFile);
      storeReconSamples();
    }
  }

  for (int puSizeIdx = 1; puSizeIdx < 4; puSizeIdx++)
  {
    int puSize = (int)pow(2, puSizeIdx + 2);
    int puSizeStep = (int)pow(puSize/8, 2);
    if ((!puSizes[puSizeIdx]) || ((blockIdx + 1) % puSizeStep != 0))
      continue;
    pu->setPuSize(puSize);
    setPuIdx(0);
    storeHeader(reconFile);
    storeReconSamples();
  }
}

ChromaPredWriter::ChromaPredWriter(std::ofstream *newPredFile, std::ofstream *newReconFile, IntraCu *cu,
                               int **recon, const ImgComp comp) : PredWriter(newPredFile, newReconFile, cu, recon)
{
  assert(comp != LUMA);
  pu->setImgComp(comp);
}

ChromaPredWriter::~ChromaPredWriter()
{
}

void ChromaPredWriter::setPuIdx()
{
  int puSizeStep = (int)pow(pu->getPuSize()/4, 2);
  int blockIdxNorm = blockIdx / puSizeStep;
  int puIdx = puSizeStep * blockIdxNorm;
  pu->setPuIdx(puIdx);
}

void ChromaPredWriter::updatePu()
{
  PredParams *predParams = PredParams::getChromaInstance();
  int puSize = predParams->getPuSize();
  int modeIdx = predParams->getModeIdx();
  if (modeIdx == 35)
    modeIdx = PredParams::getLumaMode(puSize);
  pu->setPuSize(puSize);
  pu->setModeIdx(modeIdx);
  setPuIdx();
}

void ChromaPredWriter::storeBlock()
{
  PredParams *predParams = PredParams::getChromaInstance();

  bool puSizes[] = {false, false, false};
  int puSizeStep;

  bool contChecking = true;
  while (contChecking)
  {
    updatePu();
    puSizeStep = (int)pow(pu->getPuSize()/4, 2);
    if ((blockIdx + 1) % puSizeStep == 0)
    {
      int currBlockIdx = blockIdx;
      int chromaBlockStep = (int) pow(pu->getPuSize()/4, 2);
      blockIdx -= blockIdx % chromaBlockStep;
      for (; blockIdx <= currBlockIdx; blockIdx += 4)
      {
        storeHeader(predFile);
        storePredSamples();
      }
      blockIdx = currBlockIdx;
      puSizes[log2Int(pu->getPuSize()) - 2] = true;
    }
    predParams->switchToNextPos();
    contChecking = !predParams->firstPosition();
  }

  for (int puSizeIdx = 0; puSizeIdx < 3; puSizeIdx++)
  {
    int puSize = (int)pow(2, puSizeIdx + 2);
    if (!puSizes[puSizeIdx])
      continue;
    pu->setPuSize(puSize);
    setPuIdx();
    storeHeader(reconFile);
    storeReconSamples();
  }
}

CuWriter::CuWriter(std::ofstream *newPredFile, std::ofstream *newReconFile, PicRecon *newPicRecon)
{
  assert((newPredFile != nullptr) && (newReconFile != nullptr) && (picRecon != nullptr));
  predFile = newPredFile;
  reconFile = newReconFile;
  picRecon = newPicRecon;
  cu = new IntraCu();
  blocks[0] = new LumaPredWriter(predFile, reconFile, cu, picRecon->getCompRecon(LUMA));
  blocks[1] = new ChromaPredWriter(predFile, reconFile, cu, picRecon->getCompRecon(CB), CB);
  blocks[2] = new ChromaPredWriter(predFile, reconFile, cu, picRecon->getCompRecon(CR), CR);
}

CuWriter::~CuWriter()
{
  delete cu;
  for (int comp = 0; comp < 3; comp++)
    delete blocks[comp];
}

void CuWriter::storeNextCu()
{
  bool contChecking = true;
  while (contChecking)
  {
    bool firstBlock = true;
    for (int comp = 0; comp < 3; comp++)
    {
      blocks[comp]->storeBlock();
      blocks[comp]->switchToNextBlock();
      firstBlock = firstBlock && blocks[comp]->firstBlock();
    }
    contChecking = !firstBlock;
  }
}

void CuWriter::switchToNextCu()
{
  cu->switchToNextCu();
}

bool CuWriter::picStart()
{
  return (cu->getCuX() == 0) && (cu->getCuY() == 0);
}

int DataGenerator::modes[6][7] = {{10, 26, 1, 0, 2, 18, 34}, {27, 28, 29, 30, 31, 32, 33},
                                  {25, 24, 23, 22, 21, 20, 19}, {11, 12, 13, 14, 15, 16, 17},
                                  {9, 8, 7, 6, 5, 4, 3}, {2, 18, 34, 0, 1, 10, 26}};

DataGenerator::DataGenerator(const int avPuSizes, const int avModeGroups)
{
  puSizes = avPuSizes;
  modeGroups = avModeGroups;
}

DataGenerator::~DataGenerator()
{
}

void DataGenerator::addLumaGroup(const int puSize, const int modeGroupIdx, const int subPuIdx)
{
  PredParams *predParams = PredParams::getLumaInstance(); 

  int puSizeMask = log2Int(puSize);
  if ((puSizes & puSizeMask) == 0)
    return;

  int modeGroupMask = (int)pow(2, modeGroupIdx);
  if (modeGroups & modeGroupMask)
  {
    int actualModeGroupIdx = modeGroupIdx;
    if ((puSize == 4) && (modeGroupIdx == 0))
      actualModeGroupIdx = 5;

    for (int modeIdx = 0; modeIdx < 7; modeIdx++)
      predParams->addPos(puSize, modes[actualModeGroupIdx][modeIdx], subPuIdx);
  }
}

void DataGenerator::initLumaSequence()
{
  PredParams *predParams = PredParams::getLumaInstance(); 

  addLumaGroup(4, 0, 0);
  addLumaGroup(4, 1, 0);
  addLumaGroup(4, 2, 0);
  addLumaGroup(4, 3, 0);
  addLumaGroup(4, 4, 0);

  addLumaGroup(8, 0, 0);
  addLumaGroup(8, 1, 0);
  addLumaGroup(8, 2, 0);
  addLumaGroup(8, 3, 0);
  addLumaGroup(8, 4, 0);

  addLumaGroup(16, 0, 0);
  addLumaGroup(32, 4, 0);

  addLumaGroup(4, 0, 1);
  addLumaGroup(4, 1, 1);
  addLumaGroup(4, 2, 1);
  addLumaGroup(4, 3, 1);
  addLumaGroup(4, 4, 1);

  addLumaGroup(16, 1, 0);
  addLumaGroup(32, 3, 0);
  addLumaGroup(16, 2, 0);

  addLumaGroup(4, 0, 2);
  addLumaGroup(4, 1, 2);
  addLumaGroup(4, 2, 2);
  addLumaGroup(4, 3, 2);
  addLumaGroup(4, 4, 2);

  addLumaGroup(32, 2, 0);
  addLumaGroup(16, 3, 0);
  addLumaGroup(32, 1, 0);

  addLumaGroup(4, 0, 3);
  addLumaGroup(4, 1, 3);
  addLumaGroup(4, 2, 3);
  addLumaGroup(4, 3, 3);
  addLumaGroup(4, 4, 3);

  addLumaGroup(16, 4, 0);
  addLumaGroup(32, 0, 0);

}

void DataGenerator::initChromaSequence()
{
  int chromaModes[] = {10, 26, 0, 1, 35};
  PredParams *predParams = PredParams::getChromaInstance(); 

  if (((puSizes & 1) != 0) || ((puSizes & 2) != 0))
    for (int modeIdx = 0; modeIdx < 5; modeIdx++)
      predParams->addPos(4, chromaModes[modeIdx], 0);

  if ((puSizes & 3) != 0)
    for (int modeIdx = 0; modeIdx < 5; modeIdx++)
      predParams->addPos(8, chromaModes[modeIdx], 0);
      
  if ((puSizes & 4) != 0)
    for (int modeIdx = 0; modeIdx < 5; modeIdx++)
      predParams->addPos(16, chromaModes[modeIdx], 0);

}

void DataGenerator::storeSeqHeader(std::ofstream *targetFile)
{
  SeqParams *seqParams = SeqParams::getInstance();
  *targetFile << "*************************************************" << "\n";
  *targetFile << "Picture Width       : " << seqParams->getPicWidth() << "\n";
  *targetFile << "Picture Height      : " << seqParams->getPicHeight() << "\n";
  *targetFile << "Bit Luma Depth      : " << seqParams->getBitDepthLuma() << "\n";
  *targetFile << "Bit Chroma Depth    : " << seqParams->getBitDepthChroma() << "\n";
  *targetFile << "Max CU Size         : " << seqParams->getMaxCUSize() << "\n";
  *targetFile << "Smoothing           : " << (int) seqParams->getSmoothEn() << "\n";
  *targetFile << "*************************************************" << std::endl;
}

void DataGenerator::storePredOptions(std::ofstream *targetFile)
{
  *targetFile << "*************************************************" << "\n";
  for (int puSizeMask = 1; puSizeMask < 16; puSizeMask *= 2)
  {
    int availFlag = (puSizes & puSizeMask) != 0; 
    *targetFile << "PU Sizes " << 4 * puSizeMask << "x" << 4 * puSizeMask;
    *targetFile << (puSizeMask > 2 ? " " : "   ") << "     : ";
    *targetFile << availFlag << "\n";
  }
  for (int modeGroupMask = 1, modeGroupIdx = 0; modeGroupIdx < 5; modeGroupMask *= 2, modeGroupIdx++)
  {
    int availFlag = (modeGroups & modeGroupMask) != 0;
    *targetFile << "Mode Group " << modeGroupIdx;
    *targetFile << "        : " << availFlag << "\n";
  } 
  *targetFile << "*************************************************" << std::endl;
}

void DataGenerator::storeDefLumaModes(std::ofstream *targetFile)
{
  *targetFile << "*************************************************" << "\n";
  for (int puSize = 4; puSize < 32; puSize *= 2)
  {
    *targetFile << "Luma Mode Pu " << puSize << "x" << puSize;
    *targetFile << ((puSize < 10) ? "  " : "");
    *targetFile << "  : " << PredParams::getLumaMode(puSize) << "\n";
  }
  *targetFile << "*************************************************" << std::endl;
}

void DataGenerator::storeTestData(const std::string sourceFilename, const std::string predFilename, const std::string reconFilename)
{
  PicRecon picRecon(sourceFilename);
  predFile.open(predFilename, std::ofstream::out);
  reconFile.open(reconFilename, std::ofstream::out);

  initLumaSequence();
  initChromaSequence();

  storeSeqHeader(&predFile);
  storePredOptions(&predFile);
  storeDefLumaModes(&predFile);

  storeSeqHeader(&reconFile);
  storePredOptions(&reconFile);
  storeDefLumaModes(&reconFile);

  CuWriter cuWrite(&predFile, &reconFile, &picRecon);
  for (int frameIdx = 0; frameIdx < 1; frameIdx++)
  {
    picRecon.readNextFrame();
    std::cout << "Frame " << frameIdx << ": " << std::endl;
    bool readNextCu = true;
    for (int cuIdx = 0; readNextCu; cuIdx++)
    {
      cuWrite.storeNextCu();
      std::cout << "  Intra Prediction for Cu " << cuIdx << " generated." << std::endl;
      cuWrite.switchToNextCu();
      readNextCu = !cuWrite.picStart();
    }
  }

  predFile.close();
  reconFile.close();
}
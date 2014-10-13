#include <iostream>
#include "IntraSource.h"

std::string readNextLine(std::ifstream *sourceFile)
{
  std::string line;
  std::getline(*sourceFile, line);
  if (!sourceFile->good())
  {
    std::cout << "File ended." << std::endl;
    exit(0);
  }
  return line;
}

PuReader::PuReader(CuReader *cuReader, std::ifstream *newSourceFile)
{
  assert(newSourceFile != nullptr);
  sourceFile = newSourceFile;
  pu = new IntraPu(cuReader->getCu());
  samples = nullptr;
}

PuReader::~PuReader()
{
  delete pu;
}

void PuReader::initSamples()
{
  samples = new int *[pu->getPuSize()];
  for (int x = 0; x < pu->getPuSize(); x++)
    samples[x] = new int [pu->getPuSize()];
}

void PuReader::clearSamples()
{
  if (samples != nullptr)
  {
    for (int x = 0; x < pu->getPuSize(); x++)
      delete [] samples[x];
    delete [] samples;
  }
}

void PuReader::readHeader()
{
  std::string line;
  bool search = true;
  while (search)
  {
    *sourceFile >> line;
    search = !((line == "Luma") || (line == "Cb") || (line == "Cr"));
    if (search)
      readNextLine(sourceFile);
  }
  pu->setImgComp(getImgComp(line));

  int value;
  for (int i = 0; i < 3; i++)
  {
    *sourceFile >> line >> line >> value;
    if (i == 0)
      pu->setModeIdx(value);
    else if (i == 1)
      pu->setPuIdx(value);
    else if (i == 2)
      pu->setPuSize(value);
  }
  readNextLine(sourceFile);
}

void PuReader::readSamples()
{
  for (int y = 0; y < pu->getPuSize(); y++)
  {
    for (int x = 0; x < pu->getPuSize(); x++)
      *sourceFile >> samples[x][y];
    readNextLine(sourceFile);
  }
}

void PuReader::readNextPu()
{
  clearSamples();
  readHeader();
  initSamples();
  readSamples();
}

IntraPu *PuReader::getPu() const
{
  return pu;
}

void PuReader::printSamples() const
{
  std::cout << pu->getImgComp() << " " << pu->getPuIdx() << " " << pu->getPuSize() << " ";
  std::cout << pu->getModeIdx() << std::endl;
  for (int y = 0; y < pu->getPuSize(); y++)
  {
    for (int x = 0; x < pu->getPuSize(); x++)
      std::cout << samples[x][y] << " ";
    std::cout << std::endl;
  }
}

int **PuReader::getSamples() const
{
  //printSamples();
  return samples;
}

CuReader::CuReader(std::ifstream *newSourceFile)
{
  assert(newSourceFile != nullptr);
  sourceFile = newSourceFile;
  cu = new IntraCu();
}

CuReader::~CuReader()
{
  delete cu;
}

void CuReader::switchToNextCu()
{
  cu->switchToNextCu();
}

bool CuReader::picStart()
{
  return (cu->getCuX() == 0) && (cu->getCuY() == 0);
}

IntraCu *CuReader::getCu() const
{
  return cu;
}

CuPredReader::CuPredReader(std::ifstream *newSourceFile, PicRecon *newPicRecon) : CuReader(newSourceFile)
{
  assert(newPicRecon != nullptr);
  picRecon = newPicRecon;
}

CuPredReader::~CuPredReader()
{
}

bool CuPredReader::checkPred(PuReader &puRead)
{
  IntraPu *pu = puRead.getPu();

  pu->setPicRecon(picRecon->getCompRecon(pu->getImgComp()));

  int **calcPred = pu->getPred();
  int **orgPred = puRead.getSamples();
  
  bool result = true;
  for (int x = 0; x < pu->getPuSize(); x++)
    for (int y = 0; y < pu->getPuSize(); y++)
      if (calcPred[x][y] != orgPred[x][y])
        result = false;
  
  for (int x = 0; x < pu->getPuSize(); x++)
    delete [] calcPred[x];
  delete [] calcPred;

  return result;
}

void CuPredReader::readNextCu()
{
  PuReader puRead(this, sourceFile);
  while (!cu->isCuCompleted())
  {
    puRead.readNextPu();
    assert(checkPred(puRead));
    cu->addPu(*(puRead.getPu()));
  }
}

CuReconReader::CuReconReader(std::ifstream *newSourceFile) : CuReader(newSourceFile)
{
  initSamples();
}

CuReconReader::~CuReconReader()
{
  clearSamples();
}

void CuReconReader::initSamples()
{
  SeqParams *seqParams = SeqParams::getInstance();
  samples = new int** [3];
  for (int comp = 0; comp < 3; comp++)
  {
    int cuSize = seqParams->getMaxCUSize()/(comp == LUMA ? 1 : 2);
    samples[comp] = new int* [cuSize];
    for (int x = 0; x < cuSize; x++)
      samples[comp][x] = new int [cuSize];
  }
}

void CuReconReader::clearSamples()
{
  SeqParams *seqParams = SeqParams::getInstance();
  for (int comp = 0; comp < 3; comp++)
  {
    int cuSize = seqParams->getMaxCUSize()/(comp == LUMA ? 1 : 2);
    for (int x = 0; x < cuSize; x++)
      delete [] samples[comp][x];
    delete [] samples[comp];
  }
  delete [] samples;
}

void CuReconReader::readSamples(PuReader &puRead)
{
  IntraPu *pu = puRead.getPu();
  int **source = puRead.getSamples();
  int **target = samples[pu->getImgComp()];

  int offsetX = cu->getCuX()/(pu->getImgComp() == LUMA ? 1 : 2);
  int offsetY = cu->getCuY()/(pu->getImgComp() == LUMA ? 1 : 2);

  for (int x = 0; x < pu->getPuSize(); x++)
    for (int y = 0; y < pu->getPuSize(); y++)
      target[pu->getPuX() - offsetX + x][pu->getPuY() - offsetY + y] = source[x][y];
}

void CuReconReader::printSamples()
{
  SeqParams *seqParams = SeqParams::getInstance();  
  for (int comp = 0; comp < 3; comp++)
  {
    std::cout << (comp == 0 ? "Luma:" : comp == 1 ? "Cb" : "Cr") << std::endl;
    int cuSize = seqParams->getMaxCUSize()/(comp == LUMA ? 1 : 2);
    for (int y = 0; y < cuSize; y++)
    {
      for (int x = 0; x < cuSize; x++)
        std::cout << samples[comp][x][y] << " ";
      std::cout << std::endl;
    } 
  }
}

void CuReconReader::readNextCu()
{
  PuReader puRead(this, sourceFile);
  while (!cu->isCuCompleted())
  {
    puRead.readNextPu();
    readSamples(puRead);
    cu->addPu(*(puRead.getPu()));
  } 
}

int ***CuReconReader::getSamples()
{
  //printSamples();
  return samples;
}

PicRecon::PicRecon(std::string filename)
{
  sourceFile.open(filename, std::ifstream::in);
  readSeqParams();
  initSamples();
}

PicRecon::~PicRecon()
{
  sourceFile.close();
}

void PicRecon::readSeqParams()
{
  SeqParams *seqParams = SeqParams::getInstance();
  std::regex spsStart("SPS");
  std::smatch match;
  std::string line;
  int value;

  bool search = true;
  while (search)
  {
    line = readNextLine(&sourceFile);
    if (std::regex_search(line, match, spsStart))
    {
      search = false;
      for (int lineIdx = 0; lineIdx < 6; lineIdx++)
      {
        sourceFile >> line;
        if (lineIdx == 0)
        {
          sourceFile >> line >> value;
          seqParams->setPicWidth(value);
        }
        else if (lineIdx == 1)
        {
          sourceFile >> line >> value;
          seqParams->setPicHeight(value);
        }
        else if (lineIdx == 2)
        {
          sourceFile >> line >> line >> value;
          seqParams->setBitDepthLuma(value);
        }
        else if (lineIdx == 3)
        {
          sourceFile >> line >> line >> value;
          seqParams->setBitDepthChroma(value);
        }
        else if (lineIdx == 4)
        {
          sourceFile >> line >> line >> value;
          seqParams->setMaxCUSize(value);
        }
        else if (lineIdx == 5)
        {
          sourceFile >> value;
          seqParams->setSmoothEn(value == 1 ? true : false);
        }
        readNextLine(&sourceFile);
      }
    }
  }
}

void PicRecon::initSamples()
{
  SeqParams *seqParams = SeqParams::getInstance();
  samples = new int** [3];
  for (int comp = 0; comp < 3; comp++)
  {
    int width = seqParams->getPicWidth()/(comp == LUMA ? 1 : 2);
    int height = seqParams->getPicHeight()/(comp == LUMA ? 1 : 2);
    samples[comp] = new int* [width];
    for (int x = 0; x < width; x++)
      samples[comp][x] = new int [height];
  }
}

void PicRecon::clearSamples()
{
  SeqParams *seqParams = SeqParams::getInstance();
  for (int comp = 0; comp < 3; comp++)
  {
    int width = seqParams->getPicWidth()/(LUMA ? 1 : 2);
    for (int x = 0; x < width; x++)
      delete [] samples[comp][x];
    delete [] samples[comp];
  }
  delete [] samples;
}

void PicRecon::updateSamples(CuReconReader &cuRead)
{
  int ***source = cuRead.getSamples();
  SeqParams *seqParams = SeqParams::getInstance();
  IntraCu *cu = cuRead.getCu();
  for (int comp = 0; comp < 3; comp++)
  {
    int div = comp == LUMA ? 1 : 2;
    for (int x = 0; x < cu->getCuWidth()/div; x++)
      for (int y = 0; y < cu->getCuHeight()/div; y++)
        samples[comp][cu->getCuX()/div + x][cu->getCuY()/div + y] = source[comp][x][y];
  }
}

void PicRecon::readNextFrame()
{
  CuReconReader cuRead(&sourceFile);
  bool readNextCu = true;
  while (readNextCu)
  {
    cuRead.readNextCu();
    updateSamples(cuRead);
    cuRead.switchToNextCu();
    readNextCu = !cuRead.picStart();
  }
}

int **PicRecon::getCompRecon(ImgComp comp)
{
  return samples[comp];
}
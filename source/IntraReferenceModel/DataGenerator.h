#ifndef DATA_GENERATOR_H
#define DATA_GENERATOR_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "SeqParams.h"
#include "IntraCu.h"
#include "IntraPu.h"

#include "IntraSource.h"

class PredParams
{
  private:
    struct PredPosition
    {
      int puSize, modeIdx, pu4x4Idx;
    };
  private:
    static PredParams *lumaInstance, *chromaInstance;
    std::vector<PredPosition> predPos;
    unsigned int posIdx;
    int lumaModes[3];
    PredParams();
    int selectLumaMode(const int);
  public:
    static PredParams *getLumaInstance();
    static PredParams *getChromaInstance();
    static int getLumaMode(const int);
    ~PredParams();
    void addPos(const int, const int, const int);
    void switchToNextPos();
    bool firstPosition() const;
    int getPuSize() const;
    int getModeIdx() const;
    int getPu4x4Idx() const;
};

class PredWriter
{
  protected:
    std::ofstream *predFile, *reconFile;
    int **picRecon, blockIdx;
    IntraPu *pu;
    int getBlockX() const;
    int getBlockY() const;
    std::string getImgComp() const;
    void storeHeader(std::ofstream *targetFile) const;
    void storePredSamples() const;
    void storeReconSamples() const;
  public:
    PredWriter(std::ofstream *, std::ofstream *, IntraCu *, int **);
    virtual ~PredWriter();
    virtual void storeBlock() = 0;
    void switchToNextBlock();
    bool firstBlock() const;
};

class LumaPredWriter : public PredWriter
{
  private:
    void updatePu();
    void setPuIdx(const int);
  public:
    LumaPredWriter(std::ofstream *, std::ofstream *, IntraCu *, int **);
    ~LumaPredWriter();
    void storeBlock();
};

class ChromaPredWriter : public PredWriter
{
  private:
    void updatePu();
    void setPuIdx();
  public:
    ChromaPredWriter(std::ofstream *, std::ofstream *, IntraCu *, int **, const ImgComp);
    ~ChromaPredWriter();
    void storeBlock();
};

class CuWriter
{
  private:
    PicRecon *picRecon;
    std::ofstream *predFile, *reconFile;
    IntraCu *cu;
    PredWriter *blocks[3];
  public:
    CuWriter(std::ofstream *, std::ofstream *, PicRecon *);
    ~CuWriter();
    void storeNextCu();
    void switchToNextCu();
    bool picStart();
    IntraCu *getCu() const;
};

class DataGenerator
{
  private:
    static int modes[6][7];
    std::ofstream predFile, reconFile;
    int puSizes, modeGroups;
    void addLumaGroup(const int, const int, const int);
    void initLumaSequence();
    void initChromaSequence();
    void storeSeqHeader(std::ofstream *);
    void storePredOptions(std::ofstream *);
    void storeDefLumaModes(std::ofstream *);
  public:
    DataGenerator(const int , const int);
    ~DataGenerator();
    void storeTestData(const std::string, const std::string, const std::string);
};

#endif
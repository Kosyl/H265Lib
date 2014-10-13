#ifndef INTRA_SOURCE_H
#define INTRA_SOURCE_H

#include <fstream>
#include <string>
#include <regex>

#include "SeqParams.h"
#include "IntraCu.h"
#include "IntraPu.h"

class CuReader;
class PicRecon;

std::string readNextLine(std::ifstream *);

class PuReader
{
  private:
    std::ifstream *sourceFile;
    IntraPu *pu;
    int **samples;
    void initSamples();
    void clearSamples();
    void readHeader();
    void readSamples();
  public:
    PuReader(CuReader *, std::ifstream *);
    ~PuReader();
    void readNextPu();
    IntraPu *getPu() const;
    void printSamples() const;
    int **getSamples() const;
};

class CuReader
{
  protected:
    std::ifstream *sourceFile;
    IntraCu *cu;
  public:
    CuReader(std::ifstream *);
    ~CuReader();
    void switchToNextCu();
    bool picStart();
    IntraCu *getCu() const;
};

class CuPredReader: public CuReader
{   
  private:
    PicRecon *picRecon;
    bool checkPred(PuReader &);
  public:
    CuPredReader(std::ifstream *, PicRecon *);
    ~CuPredReader();
    void readNextCu();
};

class CuReconReader: public CuReader
{
  private:
    int ***samples;
    void initSamples();
    void clearSamples();
    void readSamples(PuReader &);
  public:
    CuReconReader(std::ifstream *);
    ~CuReconReader();
    void readNextCu();
    void printSamples();
    int ***getSamples();
};

class PicRecon
{
  private:
    std::ifstream sourceFile;
    int ***samples;
    void readSeqParams();
    void initSamples();
    void clearSamples();
    void updateSamples(CuReconReader &);
  public:
    PicRecon(std::string);
    ~PicRecon();
    void readNextFrame();
    int **getCompRecon(ImgComp comp);
};

#endif
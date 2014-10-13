#include <iostream>
#include <fstream>

#include "SeqParams.h"
#include "IntraSource.h"
#include "DataGenerator.h"

void testIntra(const int frameNum);

void initLumaSequence();
void initChromaSequence();
void genData(const int frameNum);

int main()
{
  //testIntra(8);
  genData(1);
}

void testIntra(const int frameNum)
{
  PicRecon picRecon("D:\\txt\\recon.txt");

  std::ifstream predFile;
  predFile.open("D:\\txt\\intra_pred.txt", std::ifstream::in);

  CuPredReader predRead(&predFile, &picRecon);

  for (int frameIdx = 0; frameIdx < frameNum; frameIdx++)
  {
    int cuIdx = 0;
    picRecon.readNextFrame();
    std::cout << "Frame " << frameIdx << ": " << std::endl;
    bool readNextCu = true;
    while (readNextCu)
    {
      predRead.readNextCu();
      predRead.switchToNextCu();
      readNextCu = !predRead.picStart();
      std::cout << "  Cu " << cuIdx++ << " verified." << std::endl;
    }
  }

  predFile.close();
}

void genData(const int frameNum)
{
  DataGenerator dataGen(15, 31);

  dataGen.storeTestData("D:\\txt\\recon.txt", "D:\\txt\\pred_gen.txt", "D:\\txt\\recon_gen.txt");

}
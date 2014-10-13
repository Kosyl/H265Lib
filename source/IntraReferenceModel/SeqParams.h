#ifndef SEQ_PARAMS_H
#define SEQ_PARAMS_H

#include <cassert>
#include <string>

enum ImgComp {LUMA, CB, CR};
enum Direction {INTRA_DIR_LEFT = 0, INTRA_DIR_TOP, INTRA_DIR_CORNER};

class SeqParams
{
  private:
    static SeqParams *instance;
    int picWidth, picHeight, bitDepthLuma, bitDepthChroma, maxCuSize;
    bool smoothEn;
    SeqParams();
  public:
    static SeqParams* getInstance();
    ~SeqParams();
    void setPicWidth(const int);
    void setPicHeight(const int);
    void setBitDepthLuma(const int);
    void setBitDepthChroma(const int);
    void setMaxCUSize(const int);
    void setSmoothEn(const bool);
    int getPicWidth() const;
    int getPicHeight() const;
    int getBitDepthLuma() const;
    int getBitDepthChroma() const;
    int getMaxCUSize() const;
    bool getSmoothEn() const;
    int clip(const ImgComp, const int) const;
    int getDefVal(const ImgComp) const;
};

int log2Int(const int);
ImgComp getImgComp(const std::string);

#endif
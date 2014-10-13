#ifndef INTRA_CU_H
#define INTRA_CU_H

#include <cassert>

#include "SeqParams.h"

class IntraPu;

class IntraCu
{
  private:
    int cuX, cuY;
    IntraPu **puList;
    void resetPu(const ImgComp);
    bool isCompCompleted(const ImgComp) const;
  public:
    IntraCu(const int = 0, const int = 0);
    ~IntraCu();
    void setCuX(const int);
    void setCuY(const int);
    void addPu(const IntraPu &);
    int getCuX() const;
    int getCuY() const;
    int getCuWidth() const;
    int getCuHeight() const;
    bool isCuCompleted() const;
    bool switchToNextCu();
};

#endif
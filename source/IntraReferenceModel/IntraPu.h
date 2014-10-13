#ifndef INTRA_PU_H
#define INTRA_PU_H

#include <cassert>
#include "SeqParams.h"
#include "IntraCu.h"
#include "IntraPred.h"

class IntraPu
{
  private:
    const IntraCu *cu;
    ImgComp comp;
    int puIdx, puSize, modeIdx;
    int const* const* picRecon;
    int calcPuIdx(const int, const int) const;
    bool calcPuAvail(const int, const int) const;
    int getReconRef(const Direction, const int = 0) const;
    int getRefSubs(const Direction) const;
  public:
    IntraPu(const IntraCu*);
    IntraPu(const IntraPu&);
    IntraPu& operator=(const IntraPu&);
    ~IntraPu();
    void setCu(const IntraCu*);
    void setImgComp(const ImgComp);
    void setPuIdx(const int);
    void setPuSize(const int);
    void setModeIdx(const int);
    void setPicRecon(int const* const*);
    IntraCu *getCu() const;
    ImgComp getImgComp()  const;
    int getPuIdx() const;
    int getPuSize() const;
    int getModeIdx() const;
    int getPuX() const;
    int getPuY() const;
    int getCorner() const;
    int *getSideRefs(const Direction) const;
    int **getPred() const;
	int **getPredForceRef(const int *leftRefs, const int *topRefs, const int corner) const;
};

#endif
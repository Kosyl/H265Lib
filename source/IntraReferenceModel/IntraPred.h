#ifndef INTRA_PRED_H
#define INTRA_PRED_H

#include <cassert>
#include <algorithm>

#include "SeqParams.h"
#include "IntraPu.h"
#include "IntraMode.h"

class IntraPred
{
  private:
    enum ModeType {PLANAR = 0, DC, LINEAR, ANGULAR};
    static IntraPred *itsInstance;
    IntraMode **itsModes;
    const IntraPu *pu;
    int itsCornerValue;
    int *itsLeftRefs, *itsTopRefs;
    IntraPred();
    int getFilteringThreshold() const;
    bool isFilteringRequired() const;
    int filtRef(const int, const int, const int) const;
    void filterSideRefs(const Direction);
    void filter();
    bool checkSmoothCond(const Direction) const;
    bool isSmoothReq() const;
    int getSmoothedReferenceAtPosition(const Direction, const int) const;
    void smoothSideRefs(const Direction);
    void doReferenceSmoothing();
    IntraMode *getPredictionStrategy();
  public:
    ~IntraPred();
    static IntraPred *getInstance();
    int **calcPred(const IntraPu *);
	int **calcPredForceRefs(const IntraPu *, const int *leftRefs, const int *topRefs, const int corner);
};

#endif
#ifndef INTRA_MODE_H
#define INTRA_MODE_H

#include <cassert>

class IntraPu;

class IntraMode
{
  protected:
    const IntraPu *pu;
    int itsCornerReference;
    const int *leftRefs, *topRefs;
    int **initPred() const;
  public:
    IntraMode();
    virtual ~IntraMode();
    virtual int **calcPred() = 0;
    virtual void setPu(const IntraPu*);
    virtual void setCorner(const int);
    virtual void setSideRefs(const Direction, const int*);
};

class PlanarMode: public IntraMode
{
  public:
    PlanarMode();
    ~PlanarMode();
    int **calcPred();
};

class DcMode: public IntraMode
{
  private:
    int dcVal;
    void calcDcVal();
    int getFiltCorner() const;
    int getFiltEdge(const Direction, const int) const;
  public:
    DcMode();
    ~DcMode();
    int **calcPred();
};

class LinearMode: public IntraMode
{
  private:
    int getFiltEdge(const Direction dir, const int);
  public:
    LinearMode();
    ~LinearMode();
    int **calcPred();
};

class AngMode: public IntraMode
{
  private:
    const static int angles[];
    const static int invAngles[];
    int *refsArray;
    int getAngle() const;
    int getInvAngle() const;
    void getRefsArray();
  public:
    AngMode();
    ~AngMode();
    int **calcPred();
};

#endif
/**
* @file	IntraMode.h
*
* @brief	Deklaracje klasy strategii intra
*/
#ifndef INTRA_MODE_H
#define INTRA_MODE_H

#include <cassert>
#include <Common/TypeDef.h>
#include <Common/Enums.h>
#include <Data/ReferenceSamples.h>

namespace HEVC
{
	class PBIntra;

	struct IntraMode
	{
		IntraMode();
		virtual ~IntraMode();

		virtual Matrix<Sample> calcPred(ReferenceSamples samples, ImgComp img_comp, int mode_idx) = 0;
	};

	class PlanarMode : public IntraMode
	{
	public:

		PlanarMode();
		virtual ~PlanarMode();

		virtual Matrix<Sample> calcPred(ReferenceSamples samples, ImgComp img_comp, int mode_idx) override;
	};

	class DcMode : public IntraMode
	{
	private:

		Sample itsDCVal;
		bool itsFiltreEdges;

		void calcDcVal();
		Sample getFiltCorner() const;
		Sample getFiltEdge(const IntraDirection dir, const int offset) const;

	public:

		DcMode();
		virtual ~DcMode();

		virtual Matrix<Sample> calcPred(ReferenceSamples samples, ImgComp img_comp, int mode_idx) override;
	};

	class LinearMode : public IntraMode
	{
	private:

		Sample getFiltEdge(const IntraDirection dir, const int offset);
	public:

		LinearMode();
		virtual ~LinearMode();

		virtual Matrix<Sample> calcPred(ReferenceSamples samples, ImgComp img_comp, int mode_idx) override;
	};

	class AngMode : public IntraMode
	{
	private:

		const static Int angles[];

		const static Int invAngles[];

		Sample* refsArray;

		Int getAngle() const;

		Int getInvAngle() const;

		void getRefsArray();
	public:

		AngMode();
		virtual ~AngMode();

		virtual Matrix<Sample> calcPred(ReferenceSamples samples, ImgComp img_comp, int mode_idx) override;
	};
}
#endif
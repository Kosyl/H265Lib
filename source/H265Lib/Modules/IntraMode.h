#ifndef INTRA_MODE_H
#define INTRA_MODE_H

#include <cassert>
#include <Common/TypeDef.h>
#include <Common/Enums.h>
#include <Common/Matrix.h>
#include <Data/ReferenceSamples.h>

namespace HEVC
{
	class PBIntra;

	interface IntraMode
	{
		IntraMode( ) = default;
		virtual ~IntraMode() = default;

		virtual Matrix<Sample> calcPred(ReferenceSamples samples, ImgComp img_comp, int mode_idx) = 0;
	};

	class PlanarMode : public IntraMode
	{
	public:

		PlanarMode() = default;
		virtual ~PlanarMode() = default;

		virtual Matrix<Sample> calcPred(ReferenceSamples samples, ImgComp img_comp, int mode_idx) override;
	};

	class DcMode : public IntraMode
	{
	private:

		bool itsFiltreEdges;

		Sample calcDcVal( ReferenceSamples &samples );
		Sample getFiltCorner() const;
		Sample getFiltEdge(const IntraDirection dir, const int offset) const;

	public:

		DcMode() = default;
		virtual ~DcMode() = default;

		virtual Matrix<Sample> calcPred(ReferenceSamples samples, ImgComp img_comp, int mode_idx) override;
	};

	class LinearMode : public IntraMode
	{
	private:

		Sample getFiltEdge(const IntraDirection dir, const int offset);
	public:

		LinearMode() = default;
		virtual ~LinearMode() = default;

		virtual Matrix<Sample> calcPred(ReferenceSamples samples, ImgComp img_comp, int mode_idx) override;
	};

	class AngMode : public IntraMode
	{
	private:

		const static int angles[];

		const static int invAngles[];

		Sample* refsArray;

		int getAngle() const;

		int getInvAngle() const;

		void getRefsArray();
	public:

		AngMode() = default;
		virtual ~AngMode() = default;

		virtual Matrix<Sample> calcPred(ReferenceSamples samples, ImgComp img_comp, int mode_idx) override;
	};
}
#endif
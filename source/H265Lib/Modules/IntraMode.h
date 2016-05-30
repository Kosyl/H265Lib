#ifndef INTRA_MODE_H
#define INTRA_MODE_H

#include <cassert>
#include <Common/TypeDef.h>
#include <Common/Enums.h>
#include <Common/Matrix.h>
#include <ParameterSets/SequenceParameterSet.h>
#include <Data/ReferenceSamples.h>

namespace HEVC
{
	class PBIntra;

	interface IntraMode
	{
		IntraMode( ) = default;
		virtual ~IntraMode() = default;

		virtual Matrix<Sample> calcPred(IntraReferenceSamples samples, ImgComp img_comp, int mode_idx) = 0;
	};

	class PlanarMode : public IntraMode
	{
	public:
		PlanarMode() = default;
		virtual ~PlanarMode() = default;

		virtual Matrix<Sample> calcPred(IntraReferenceSamples samples, ImgComp img_comp, int mode_idx) override;
	};

	class DcMode : public IntraMode
	{
	private:
		Sample calcDcValue( IntraReferenceSamples &samples );

	public:
		DcMode() = default;
		virtual ~DcMode() = default;

		virtual Matrix<Sample> calcPred(IntraReferenceSamples samples, ImgComp img_comp, int mode_idx) override;
	};

	class LinearMode : public IntraMode
	{
	private:
		std::shared_ptr<SequenceParameterSet> sps;
		Sample getFiltEdge(IntraReferenceSamples &samples, const IntraDirection dir, const ImgComp img_comp, const int offset);

	public:
		LinearMode() = default;
		virtual ~LinearMode() = default;

		virtual Matrix<Sample> calcPred(IntraReferenceSamples samples, ImgComp img_comp, int mode_idx) override;
	};

	class AngMode : public IntraMode
	{
	private:

		const static int angles[];

		const static int invAngles[];

		int getAngle(int mode_idx) const;
		int getInvAngle(int mode_idx) const;

		std::vector<Sample> getRefsArray(IntraReferenceSamples &samples, int mode_idx);
	public:

		AngMode() = default;
		virtual ~AngMode() = default;

		virtual Matrix<Sample> calcPred(IntraReferenceSamples samples, ImgComp img_comp, int mode_idx) override;
	};
}
#endif
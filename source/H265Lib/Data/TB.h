#pragma once

#include <Common/Bitset.h>
#include <Data/BlockBase.h>
#include <Data/ReferenceSamples.h>

namespace H265Lib
{
	class TB : public BlockBase
	{
	private:

		ImgComp _plane;

		ReferenceSamples _referenceSamples;

		Matrix<Sample> _inputSamples;
		Matrix<Sample> _prediction;
		Matrix<Sample> _residuals;
		Matrix<Coeff> _transformCoefficients;
		Matrix<Sample> _reconstructedSamples;

		UInt _lastSignificantCoeffX, _lastSignificantCoeffY;
		Bitset _codedSubBlockFlags;
		UInt _distortion;
		UInt _numBins;

	public:

		TB(UShort x, UShort y, UShort size, ImgComp comp, ParametersBundle parameters);
		~TB() = default;

		Matrix<Coeff> getCoefficients() const
		{
			return _transformCoefficients;
		}

		Bool TB::hasAtLeastOneNonZeroCoeff();

		virtual Void printDescription(Logs logId, Bool recursive = true) override;
	};
}
#pragma once

#include <Common/Bitset.h>
#include <Data/BlockBase.h>

namespace H265Lib
{
	class TB : public BlockBase
	{
		ImgComp _plane;

		Matrix<Sample> _inputSamples;

		Matrix<Sample> _prediction;

		Matrix<Sample> _residuals;

		Matrix<Sample> _reconstructedSamples;

		Matrix<Coeff> _transformCoefficients;

		//ScanningMode* itsScanStrategy;

		UInt _lastSignificantCoeffX, _lastSignificantCoeffY;

		Bitset _codedSubBlockFlags;

		UInt _distortion;

		UInt _numBins;

	public:

		TB(UShort x, UShort y, UShort size, ImgComp comp, ParametersBundle parameters);

		~TB();
		/*
		Void assignScanStrategy();
		*/
		/*
		ScanningMode* getScanStrategy() const
		{
		return itsScanStrategy;
		}
		*/
		Matrix<Coeff> getCoefficients() const
		{
			return _transformCoefficients;
		}

		void forceCoefficients(Matrix<Coeff> val)
		{
			_transformCoefficients = val;
		}

		//Bool hasAtLeastOneNonZeroCoeff();
		virtual Void printDescription(Logs logId) override;
	};
}
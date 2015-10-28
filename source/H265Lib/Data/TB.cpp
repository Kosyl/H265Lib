#include "TB.h"

namespace H265Lib
{
	TB::TB(UShort x, UShort y, UShort size, ImgComp comp, ParametersBundle parameters) :
		BlockBase(x, y, size, parameters),
		_plane(comp),
		//itsScanStrategy(nullptr),
		_codedSubBlockFlags(),
		_lastSignificantCoeffX(0),
		_lastSignificantCoeffY(0)
	{
	}

	TB::~TB()
	{
	}

	/*Void TB::assignScanStrategy()
	{
	assert(itsHelperPB != nullptr && itsHelperPB->predictionDone());
	itsScanStrategy = new DiagonalScanningMode();
	if (itsSize == 4 || (itsSize == 8 && itsComp == LUMA))
	{
	UInt mode = itsHelperPB->getModeIdx();
	if (mode >= 6 && mode <= 14)
	{
	delete itsScanStrategy;
	itsScanStrategy = new VerticalScanningMode();
	}
	else if (mode >= 22 && mode <= 30)
	{
	delete itsScanStrategy;
	itsScanStrategy = new HorizontalScanningMode();
	}
	}
	}*/

	Bool TB::hasAtLeastOneNonZeroCoeff()
	{
		for (auto& coeff : _transformCoefficients)
		{
			if (coeff != 0)
				return true;
		}
		return false;
	}

	Void TB::printDescription(Logs logId, Bool recursive)
	{
		BlockBase::printDescription(logId, recursive);
	}
}
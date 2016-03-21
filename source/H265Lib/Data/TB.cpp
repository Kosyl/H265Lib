#include "TB.h"

namespace HEVC
{
	TB::TB(int x, int y, int size, ImgComp comp) :
		BlockBase(x, y, size),
		plane(comp),
		reference_samples(size)
	{
	}

	/*void TB::assignScanStrategy()
	{
	assert(itsHelperPB != nullptr && itsHelperPB->predictionDone());
	itsScanStrategy = new DiagonalScanningMode();
	if (itsSize == 4 || (itsSize == 8 && itsComp == LUMA))
	{
	int mode = itsHelperPB->getModeIdx();
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

	bool TB::hasAtLeastOneNonZeroCoeff()
	{
		for (auto& coeff : transform_coefficients)
		{
			if (coeff != 0)
				return true;
		}
		return false;
	}

	void TB::print(LogId logId, bool recursive)
	{
		BlockBase::print(logId, recursive);
	}
}
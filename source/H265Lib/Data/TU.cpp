#include "TU.h"

namespace H265Lib
{
	TU::TU(UInt x, UInt y, UInt size, ParametersBundle parameters) :
		BlockBase(x, y, size)
	{
		/*assert(size == 4 || size == 8 || size == 16 || size == 32);
		itsTBs.push_back(std::make_shared<TB>(x, y, size, Luma));

		if (size == _sps->getMinTUSize())
		{
		if (x % 8 == 0 && y % 8 == 0)
		{
		itsTBs.push_back(std::make_shared<TB>(x / 2, y / 2, size, Cb));
		itsTBs.push_back(std::make_shared<TB>(x / 2, y / 2, size, Cr));
		}
		}
		else
		{
		itsTBs.push_back(std::make_shared<TB>(x / 2, y / 2, size / 2, Cb));
		itsTBs.push_back(std::make_shared<TB>(x / 2, y / 2, size / 2, Cr));
		}*/
	}
}
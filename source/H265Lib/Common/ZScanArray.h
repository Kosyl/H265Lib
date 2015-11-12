#pragma once

#include <Common/Calc.h>
#include <Common/Enums.h>
#include <Common/Matrix.h>

namespace H265Lib
{
	class ZScanArray
	{
		Matrix<UInt> _values;

		UInt _sizeX, _sizeY;

	public:

		Void reset();

		UInt get(UInt x, UInt y);

		Void print(std::ostream& out);
	};
}
/**
 * @file	SeqPicParameters.cpp
 *
 * @brief	Implementacja klas parametrów sekwencji i obrazu
 */

#include "ZScanArray.h"

namespace H265Lib
{


	UInt ZScanArray::get(UInt x, UInt y)
	{
		if (x < _sizeX && y < _sizeY)
			return _values.at(x, y);
		else
			return -1;
	}
}
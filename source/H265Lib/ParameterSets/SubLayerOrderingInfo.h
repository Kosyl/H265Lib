#pragma once
#include <Common/TypeDef.h>

namespace H265Lib
{
	class SubLayerOrderingInfo
	{
		UShort _maxDecPicBuffering;
		UShort _maxNumReorderPic;
		UShort _maxLatencyIncrease;
	};
}
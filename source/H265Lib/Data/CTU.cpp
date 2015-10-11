#include "CTU.h"
#include "CU.h"

namespace H265Lib
{
	CTU::CTU(UShort x, UShort y, UShort size, ParametersBundle parameters) :
		BlockBase(x, y, size, parameters),
		_CUQuadTree(std::make_shared<CUQuadTree>(x, y, size, parameters))
	{
	}

	CTU::~CTU()
	{
	}

	Void CTU::printDescription()
	{
	}
}
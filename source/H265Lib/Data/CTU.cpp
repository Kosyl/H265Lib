#include "CTU.h"
#include "CU.h"

namespace H265Lib
{
	CTU::CTU(UShort x, UShort y, UShort size, ParametersBundle parameters) :
		BlockBase(x, y, size),
		_CUQuadTree(std::make_shared<CUQuadTree>(x, y, size, parameters))
	{
	}

	std::shared_ptr<CUQuadTree> CTU::getCUQuadTree()
	{
		return _CUQuadTree;
	}

	Bool CTU::isPartitioned()
	{
		return _CUQuadTree != nullptr;
	}

	Void CTU::printDescription(LogId logId, Bool recursive)
	{
	}
}
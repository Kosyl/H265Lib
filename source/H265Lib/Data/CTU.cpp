#include "CTU.h"
#include "CU.h"

namespace HEVC
{
	CTU::CTU(int x, int y, int size) :
		BlockBase(x, y, size),
		CUQuadTree(std::make_shared<HEVC::CUQuadTree>(x, y, size))
	{
	}

	bool CTU::isPartitioned()
	{
		return CUQuadTree != nullptr;
	}

	void CTU::print(LogId logId, bool recursive)
	{
	}
}
#include "CTU.h"
#include "CU.h"

namespace HEVC
{
	CTU::CTU(int x, int y, int size) :
		BlockBase(x, y, size),
		cu_tree(std::make_unique<HEVC::CUQuadTree>(x, y, size))
	{
	}

	void CTU::print(LogId logId, bool recursive)
	{
	}
}

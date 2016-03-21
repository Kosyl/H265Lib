#pragma once

#include "SAOInfo.h"
#include "Picture.h"
#include "QuadTree.h"
#include "BlockBase.h"

namespace HEVC
{
	class CTU : public BlockBase
	{
	public:

		CTU(int x, int y, int size);
		virtual ~CTU() = default;

		std::shared_ptr<CUQuadTree> CUQuadTree;
		bool isPartitioned();

		virtual void print(LogId logId, bool recursive = true) override;
	};
}
#pragma once

#include "QuadTree.h"
#include "BlockBase.h"

namespace HEVC
{
	class CTU : public BlockBase
	{
	private:

	public:
		std::shared_ptr<Slice> slice;
		std::unique_ptr<CUQuadTree> cu_tree;

		CTU(int x, int y, int size);
		virtual ~CTU() = default;
		
		virtual void print(LogId logId, bool recursive = true) override;
	};
}
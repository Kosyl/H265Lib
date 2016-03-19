#pragma once

#include "SAOInfo.h"
#include "Picture.h"
#include "QuadTree.h"
#include "BlockBase.h"

namespace H265Lib
{
	class CTU : public BlockBase
	{
	private:

		std::shared_ptr<CUQuadTree> _CUQuadTree;

	public:

		CTU(UShort x, UShort y, UShort size, ParametersBundle parameters);
		virtual ~CTU() = default;

		std::shared_ptr<CUQuadTree> getCUQuadTree();
		Bool isPartitioned();

		virtual Void printDescription(LogId logId, Bool recursive = true) override;
	};
}
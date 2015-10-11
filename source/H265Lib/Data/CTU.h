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

		virtual ~CTU();

		std::shared_ptr<CUQuadTree> getCUQuadTree()
		{
			return _CUQuadTree;
		}

		Bool isPartitioned()
		{
			return _CUQuadTree != nullptr;
		}

		virtual Void printDescription();
	};
}
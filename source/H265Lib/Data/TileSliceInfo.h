#pragma once

#include <vector>
#include <Common/TypeDef.h>
#include <Common/Matrix.h>

namespace H265Lib
{
	class TileSliceInfo
	{
	protected:

		std::vector<std::shared_ptr<Tile> > itsTiles;

		std::vector<std::shared_ptr<Slice> > itsSlices;

		std::vector<std::shared_ptr<CTU> > itsCTUs;

	public:

	};
}
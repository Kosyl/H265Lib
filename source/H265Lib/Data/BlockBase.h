#pragma once

#include <Common/TypeDef.h>
#include <cassert>
#include <ParameterSets/SequenceParameterSet.h>
#include <Common/Logger.h>
#include <ParameterSets/ParametersBundle.h>

namespace H265Lib
{
	struct Position
	{
		UShort X, Y;
		UInt Idx;

		Position() :
			Position(0, 0)
		{

		}

		Position(UShort x, UShort y) :
			X(x), Y(y), Idx(0)
		{

		}

		Position(UShort x, UShort y, const SequenceParameterSet& sps, Indexing idxType) :
			X(x), Y(y), Idx(sps.calcIdx(x, y, idxType))
		{

		}

		void resolveIdx(const SequenceParameterSet& sps, Indexing idxType)
		{
			this->Idx = sps.calcIdx(this->X, this->Y, idxType);
		}
	};

	class BlockBase
	{
	protected:

		UShort _size;

	public:

		ParametersBundle Parameters;

		BlockBase() :
			BlockBase(0, 0, 0, ParametersBundle())
		{

		}

		BlockBase(UShort x, UShort y, UShort size, ParametersBundle parameters) :
			PositionInPicture(x, y),
			_size(size),
			Parameters(parameters)
		{
			assert(size > 0 && size % 4 == 0);
		}

		virtual ~BlockBase()
		{
		}

		Position PositionInPicture;

		virtual Void printDescription(Logs logId)
		{
		}

		UShort getSize()
		{
			return _size;
		}
	};
}
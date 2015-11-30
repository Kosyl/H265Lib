#pragma once

#include <Common/TypeDef.h>
#include <ParameterSets/SequenceParameterSet.h>
#include <Common/Logger.h>
#include <ParameterSets/ParametersBundle.h>

namespace H265Lib
{
	struct Position
	{
		UShort X, Y;
		UInt Idx;

		Position();
		Position(UShort x, UShort y);
		Position(UShort x, UShort y, const SequenceParameterSet& sps, Indexing idxType);

		Void resolveIdx(const SequenceParameterSet& sps, Indexing idxType);
	};

	class BlockBase
	{
	protected:

		UShort _size;

	public:

		Position Position;

		BlockBase();
		BlockBase(UShort x, UShort y, UShort size);
		virtual ~BlockBase() = default;

		UShort getSize();

		virtual Void printDescription(LogId logId, Bool recursive = true);
	};
}
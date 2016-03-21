#pragma once

#include <Common/TypeDef.h>
#include <ParameterSets/SequenceParameterSet.h>
#include <ParameterSets/ParametersBundle.h>

namespace HEVC
{
	struct Position
	{
		int x, y;
		int idx;

		Position();
		Position(int x, int y);

		void resolveIdx(const SequenceParameterSet& sps, Indexing idxType);
	};

	class BlockBase: public Position
	{
	public:

		BlockBase();
		BlockBase(int x, int y, int size);
		virtual ~BlockBase() = default;

		int size;

		virtual void print(LogId logId, bool recursive);
	};
}
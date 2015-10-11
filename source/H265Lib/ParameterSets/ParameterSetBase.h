#pragma once

#include "Common/TypeDef.h"

namespace H265Lib
{
	class ParameterSetBase
	{
	private:
		UShort _idx;

	public:
		virtual ~ParameterSetBase()
		{
		}

		ParameterSetBase(UShort idx) :
			_idx(idx)
		{

		}

		UShort getParameterSetIdx()
		{
			return _idx;
		}

		virtual void initWithDefaults() = 0;
	};
}
#pragma once

#include "Common/TypeDef.h"

namespace HEVC
{
	class ParameterSetBase
	{
	private:
		int _idx;

	public:
		virtual ~ParameterSetBase()
		{
		}

		ParameterSetBase(int idx) :
			_idx(idx)
		{

		}

		int getParameterSetIdx()
		{
			return _idx;
		}

		virtual void initWithDefaults() = 0;
	};
}
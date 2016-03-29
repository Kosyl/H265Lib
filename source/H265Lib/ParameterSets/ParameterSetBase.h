#pragma once

#include "Common/TypeDef.h"

namespace HEVC
{
	class ParameterSetBase
	{
	private:

	public:
		virtual ~ParameterSetBase()
		{
		}

		ParameterSetBase(int idx) :
			Id(idx)
		{

		}

		int Id;

		virtual void initWithDefaults() = 0;
	};
}
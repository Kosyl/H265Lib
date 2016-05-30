#pragma once
#include "Common/Singleton.h"
#include <Common\TypeDef.h>
#include <unordered_map>
#include <memory>

namespace HEVC
{
	template < typename TParameterClass, typename TSelf >
	class ParameterBank : public Singleton < TSelf >
	{
	protected:
		std::unordered_map<int, std::shared_ptr<TParameterClass>> parameter_sets;
		int last_set_idx = 0;

		int getNextIdx( )
		{
			return ++last_set_idx;
		}

	public:

		std::shared_ptr<TParameterClass> getSetByIdx(int idx)
		{
			return parameter_sets[idx];
		}

		std::shared_ptr<TParameterClass> getCurrent()
		{
			return parameter_sets[last_set_idx];
		}
	};
}
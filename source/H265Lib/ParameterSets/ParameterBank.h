#pragma once
#include "Common/Singleton.h"
#include <Common\TypeDef.h>
#include <unordered_map>
#include <memory>

namespace H265Lib
{
	template < typename TParameterClass >
	class ParameterBank : public Singleton < ParameterBank<TParameterClass> >
	{
	private:
		std::unordered_map<UShort, std::shared_ptr<TParameterClass>> _parameterSets;
		UShort _lastSetIdx = 0;
		UShort _nextSetIdx = 0;

	public:
		std::shared_ptr<TParameterClass> createNext()
		{
			auto idx = _nextSetIdx;
			_lastSetIdx = idx;
			++_nextSetIdx;

			this->_parameterSets[idx] = std::make_unique<TParameterClass>(idx);
			return _parameterSets[idx];
		}

		std::shared_ptr<TParameterClass> getSetByIdx(UShort idx)
		{
			return _parameterSets[idx];
		}

		std::shared_ptr<TParameterClass> getCurrent()
		{
			return _parameterSets[_lastSetIdx];
		}
	};
}
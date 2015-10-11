#include "stdafx.h"
#include "UnitTestLogger.h"

namespace Microsoft {
	namespace VisualStudio {
		namespace CppUnitTestFramework
		{
			std::stringstream& Log::getStream()
			{
				return _logStream;
			}

			Void Log::printSpaces()
			{
				for (UInt i = 0; i < _numTabs; ++i)
					_logStream << "  ";
			}

			Log::Log():
				_numTabs(0),
				_step(2),
				_spaces("")
			{
			}

			Void Log::increaseSpaces()
			{
				setTabLength(_numTabs + 1);
			}

			Void Log::decreaseSpaces()
			{
				setTabLength(_numTabs - 1);
			}

			Void Log::setTabLength(Int len)
			{
				if (len >= 0)
				{
					_numTabs = len;
				}
				else
					setTabLength(0);
			}

			UInt Log::getTabLength()
			{
				return _numTabs;
			}

			Void Log::setTabStep(UInt len)
			{
				_step = len;
				setTabLength(_numTabs);
			}

			Void Log::tab()
			{
				auto& log = Log::instance();
				log.printSpaces();
				log.getStream() << "{" << std::endl;
				log.increaseSpaces();
			}

			Void Log::untab()
			{
				auto& log = Log::instance();
				log.decreaseSpaces();
				log.printSpaces();
				log.getStream() << "}" << std::endl;
			}
		}
	}
}
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

			void Log::printSpaces()
			{
				for (int i = 0; i < _numTabs; ++i)
					_logStream << "  ";
			}

			Log::Log():
				_numTabs(0),
				_step(2),
				_spaces("")
			{
			}

			void Log::increaseSpaces()
			{
				setTabLength(_numTabs + 1);
			}

			void Log::decreaseSpaces()
			{
				setTabLength(_numTabs - 1);
			}

			void Log::setTabLength(int len)
			{
				if (len >= 0)
				{
					_numTabs = len;
				}
				else
					setTabLength(0);
			}

			int Log::getTabLength()
			{
				return _numTabs;
			}

			void Log::setTabStep(int len)
			{
				_step = len;
				setTabLength(_numTabs);
			}

			void Log::tab()
			{
				auto& log = Log::instance();
				log.printSpaces();
				log.getStream() << "{" << std::endl;
				log.increaseSpaces();
			}

			void Log::untab()
			{
				auto& log = Log::instance();
				log.decreaseSpaces();
				log.printSpaces();
				log.getStream() << "}" << std::endl;
			}
		}
	}
}
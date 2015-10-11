#pragma once

#include <iostream>
#include "Common/TypeDef.h"
#include <memory>
#include <vector>
#include <map>
#include <sstream>
#include "Common\Singleton.h"
//#include "C:/Program Files (x86)/Microsoft Visual Studio 12.0/VC/UnitTest/include/CppUnitTestLogger.h"

namespace Microsoft {
	namespace VisualStudio {
		namespace CppUnitTestFramework
		{
			struct NewLineToken
			{

			};

			class Log : public Singleton < Log >
			{
			private:

				std::stringstream _logStream;

				std::string _spaces;

				UInt _numTabs;

				UInt _step;

				template<typename T>
				Void write(T s)
				{
					_logStream << s;
				}

				template<>
				Void write(NewLineToken)
				{
					_logStream << std::endl;
					printSpaces();
				}

				template<typename T>
				void printValues(T&& arg)
				{
					write(std::forward<T>(arg));
				}

				template<typename T1, typename... Tn>
				void printValues(T1&& arg1, Tn&&... args)
				{
					write(std::forward<T1>(arg1));
					printValues(std::forward<Tn>(args)...);
				}

				std::stringstream& getStream();

				Void printSpaces();

				Void finish()
				{
					Logger::WriteMessage(getStream().str().c_str());
					getStream().str("");
				}

			public:

				Log();

				Void increaseSpaces();

				Void decreaseSpaces();

				Void setTabLength(Int len);

				UInt getTabLength();

				Void setTabStep(UInt len);

				template<typename... Params>
				static Void println(Params... args)
				{
					auto& log = instance();
					log.printSpaces();
					log.printValues(args...);
					log.finish();
				}

				template<typename T, template<class> class TMatrix>
				static Void printMatrix(TMatrix<T>& matrix)
				{
					auto& log = instance();
					for (size_t y = 0; y < matrix.height(); ++y)
					{
						log.printSpaces();
						for (size_t x = 0; x < matrix.width(); ++x)
						{
							log.printValues(matrix.at(x, y), " ");
						}
						log.finish();
					}
				}

				template<typename... Params>
				static Void print(Params... args)
				{
					auto& log = instance();
					log.printValues(args...);
				}

				static Void tab();

				static Void untab();
			};



			/*template<typename T>
			Void printArrayToLog(Logs id, const char* name, T** matrix, int sizeX, int sizeY)
			{
			auto log = LoggingControl::instance().logs[id];
			if (log == nullptr)
			return;
			log->printSpaces();
			log->printValues(name);
			log->getStream() << std::endl;
			for (int i = 0; i < sizeY; ++i)
			{
			log->printSpaces();
			for (int j = 0; j < sizeX; ++j)
			{
			log->printValues(matrix[i][j], " ");
			}
			log->getStream() << std::endl;
			}
			}

			template<typename T>
			Void printArrayToLog1Dto2D(Logs id, const char* name, T* matrix, int sizeX, int sizeY, int stride)
			{
			auto log = LoggingControl::instance().logs[id];
			if (log == nullptr)
			return;
			log->printSpaces();
			log->printValues(name);
			log->getStream() << std::endl;
			for (int i = 0; i < sizeY; ++i)
			{
			log->printSpaces();
			for (int j = 0; j < sizeX; ++j)
			{
			log->printValues(matrix[i*stride + j], " ");
			}
			log->getStream() << std::endl;
			}
			}

			template<typename T, template<class> class TMatrix>
			void log_printMatrix(Logs logId, TMatrix<T>& matrix)
			{
			auto log = LoggingControl::instance().logs[logId];
			if (log == nullptr)
			return;
			for (size_t y = 0; y < matrix.rows(); ++y)
			{
			log->printSpaces();
			for (size_t x = 0; x < matrix.columns(); ++x)
			{
			log->printValues(matrix.at(x, y), " ");
			}
			log->getStream() << std::endl;
			}
			}


			template<typename T, template<class> class TMatrix, template<class> class SptrTMatrix>
			void log_printMatrix(Logs logId, SptrTMatrix<TMatrix<T>> pmatrix)
			{
			auto log = LoggingControl::instance().logs[logId];
			if (log == nullptr)
			return;
			for (size_t y = 0; y < pmatrix->rows(); ++y)
			{
			log->printSpaces();
			for (size_t x = 0; x < pmatrix->columns(); ++x)
			{
			log->printValues(pmatrix->at(x, y), " ");
			}
			log->getStream() << std::endl;
			}
			}*/

#ifdef _DEBUG

#define STRINGIZE(x) #x 
#define PRINTVAR(x) (#x),": ",(x),NewLineToken()
#define BINARIZE(x,numBits) (#x),": ",std::bitset<numBits>(x),NewLineToken()
			/*
		#define LOGLN(...) printlnToLog(__VA_ARGS__)
		#define LOG(...) printToLog(__VA_ARGS__)
		#define LOG_OFF(logId) LoggingControl::instance().turnOff(logId)
		#define LOG_ON(logId) LoggingControl::instance().turnOn(logId)

		#define GET_MACRO(_1,_2,_3,_4,NAME,...) NAME
		#define LOG_ARRAY_SQUARE(logId,x,mat_size) printArrayToLog(logId,(#x),(x),mat_size,mat_size)
		#define LOG_ARRAY_RECT(logId,x,mat_sizeX, mat_sizeY) printArrayToLog(logId,(#x),(x),mat_sizeX,mat_sizeY)
		#define LOG_ARRAY_1D(logId,x,mat_sizeX, mat_sizeY, stride) printArrayToLog1Dto2D(logId,(#x),(x),mat_sizeX,mat_sizeY, stride)
		#define LOG_ARRAY(...) GET_MACRO(__VA_ARGS__, LOG_ARRAY_RECT, LOG_ARRAY_SQUARE)(__VA_ARGS__)

		#define COMBINE1(a,b) a##b
		#define COMBINE(a,b) COMBINE1(a,b)
		#define LOG_SCOPE_INDENT(logId,title) Indent COMBINE(logIndent,__LINE__)(title,logId)
		#define LOG_FUNCTION_INDENT(logId) Indent COMBINE(logIndent,__LINE__)(__FUNCTION__,logId)
		#define LOG_SCOPE_MUTE(logId) Mute COMBINE(logMute,__LINE__)(logId)

		#define LOG_MATRIX(log,matrix) log_printMatrix(log, matrix)
		*/
#else
#define LOG(...)
#define LOG_MATRIX(logId,x,mat_size) 
#define LOG_MATRIX(logId,x,mat_sizeX, mat_sizeY)
#endif


		}
	}
}
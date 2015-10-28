#pragma once

#include <iostream>
#include <fstream>
#include "Common/TypeDef.h"
#include <memory>
#include <vector>
#include <map>
#include "Common\Singleton.h"

#define LOGPATH_FT "D:\\txt\\ft.txt"
#define LOGPATH_IT "D:\\txt\\it.txt"
#define LOGPATH_Q "D:\\txt\\q.txt"
#define LOGPATH_IQ "D:\\txt\\iq.txt"
#define LOGPATH_LP "D:\\txt\\lp.txt"
#define LOGPATH_LR "D:\\txt\\lr.txt"
#define LOGPATH_CP "D:\\txt\\cp.txt"
#define LOGPATH_CR "D:\\txt\\cr.txt"
#define LOGPATH_RDO "D:\\txt\\rdo.txt"
#define LOGPATH_EC "D:\\txt\\ec.txt"
#define LOGPATH_EC_SUMMARY "D:\\txt\\ec_summary.txt"
#define LOGPATH_FDB "D:\\txt\\fdb.txt"
#define LOGPATH_FDBRES "D:\\txt\\HMfdbres.txt"
#define LOGPATH_PRED "D:\\txt\\predykcja.txt"
#define LOGPATH_OVERVIEW "D:\\txt\\overview.txt"
#define LOGPATH_BINOUT "D:\\txt\\binout.txt"
#include "Common/Matrix.h"

namespace H265Lib
{
	struct NewLineToken
	{

	};

	enum class Logs
	{
		Off = 0,
		Console = 1000,
		Dump = 1001,
		ForwardTransform = 0,//1,
		InverseTransform = 0,//2,
		Quant = 3,
		Dequant = 0,//4,
		LumaPrediction = 0,//6
		LumaReco = 0,//6,
		ChromaPred = 0,//7,
		ChromaReco = 0,//8,
		DeblockingFilter = 0,//9,
		DeblockingFilterResult = 0,//10,
		RDO = 0,//11,
		Binarization = 0,//12,
		BinarizationSummary = 0,//13,
		Prediction = 0,//14,
		Overview = 0,//15,
		BinOut = 0//16
	};

	class Logger
	{
	private:

		std::string _logPath;
		std::ostream* _logStream;
		std::string _spaces;
		UInt _numTabs;

		UInt _step;
		Bool _shouldDeleteStream;

		template<typename T>
		Void write(T s);

		template<>
		Void write(NewLineToken);

	public:

		Logger(std::string logPath);

		Logger();

		static std::shared_ptr<Logger> createFileLogger(std::string logPath);

		static std::shared_ptr<Logger> createConsoleLogger();

		Void printSpaces();

		~Logger();

		Void increaseSpaces();

		Void decreaseSpaces();

		Void setTabLength(Int len);

		UInt getTabLength();

		Void setTabStep(UInt len);

		std::ostream& getStream();

		template<typename T>
		void printValues(T&& arg);

		template<typename T1, typename... Tn>
		void printValues(T1&& arg1, Tn&&... args);
	};

	class LoggingControl : public Singleton < LoggingControl >
	{
	private:

		std::vector<std::string> logNames;
		std::map<Logs, std::shared_ptr<Logger>> swapBuffer;

		void LoadSettings();

		std::shared_ptr<Logger> createLog(Logs logId, std::string path);

	public:

		LoggingControl();

		static std::string mainSettingsPath;

		~LoggingControl();

		std::map<Logs, std::shared_ptr<Logger>> logs;
		std::map<Logs, int> muteStack;

		void turnOff(Logs logId);
		void turnOn(Logs logId);
	};

	template<typename... Params>
	Void printlnToLog(Logs id, Params... args);

	template<typename... Params>
	Void printToLog(Logs id, Params... args);

	template<typename T>
	Void printArrayToLog(Logs id, const char* name, T** matrix, int sizeX, int sizeY);

	template<typename T>
	Void printArrayToLog1Dto2D(Logs id, const char* name, T* matrix, int sizeX, int sizeY, int stride);

	template<typename T, template<class> class TMatrix>
	void log_printMatrix(Logs logId, TMatrix<T>& matrix);

	template<typename T, template<class> class TMatrix, template<class> class SptrTMatrix>
	void log_printMatrix(Logs logId, SptrTMatrix<TMatrix<T>> pmatrix);

#pragma region Impl


	template<typename... Params>
	Void printlnToLog(Logs id, Params... args)
	{
		auto log = LoggingControl::instance().logs[id];
		if (log == nullptr)
			return;
		log->printSpaces();
		log->printValues(args...);
		log->getStream() << std::endl;
	}

	template<typename... Params>
	Void printToLog(Logs id, Params... args)
	{
		auto log = LoggingControl::instance().logs[id];
		if (log == nullptr)
			return;
		log->printSpaces();
		log->printValues(args...);
	}

	template<typename T>
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
	}

	template<typename T>
	Void Logger::write(T s)
	{
		*_logStream << s;
	}

	template<>
	Void Logger::write(NewLineToken)
	{
		*_logStream << std::endl;
		printSpaces();
	}

	template<typename T>
	void Logger::printValues(T&& arg)
	{
		write(std::forward<T>(arg));
	}

	template<typename T1, typename... Tn>
	void Logger::printValues(T1&& arg1, Tn&&... args)
	{
		write(std::forward<T1>(arg1));
		printValues(std::forward<Tn>(args)...);
	}

#pragma endregion

#ifdef _DEBUG

#define STRINGIZE(x) #x 
#define PRINTVAR(x) (#x),": ",(x),NewLineToken()
#define BINARIZE(x,numBits) (#x),": ",std::bitset<numBits>(x),NewLineToken()

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

#else
#define LOG(...)
#define LOG_MATRIX(logId,x,mat_size) 
#define LOG_MATRIX(logId,x,mat_sizeX, mat_sizeY)
#endif

	class Indent
	{
	private:
		Logs m_id;

		Void tab(Logs logId);
		Void untab(Logs logId);

	public:
		Indent(Logs inId);
		Indent(const char* func, Logs inId);

		~Indent();
	};

	class Mute
	{
		Logs m_id;
	public:
		Mute(Logs inId);
		~Mute();
	};
}
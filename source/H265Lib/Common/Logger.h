#pragma once

#include <iostream>
#include "Common/TypeDef.h"
#include <memory>
#include <vector>
#include <map>
#include "Common\Singleton.h"

namespace H265Lib
{
	struct NewLineToken
	{

	};

	using LogId = std::string;

	class Logger
	{
	private:

		String _logPath;
		std::ostream* _logStream;

		String _spaces;
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
		~Logger();

		static std::shared_ptr<Logger> createFileLogger(std::string logPath);
		static std::shared_ptr<Logger> createConsoleLogger();

		static const LogId Off;
		static const LogId Console;
		static const LogId Dump;
		static const LogId ForwardTransform;
		static const LogId InverseTransform;
		static const LogId Quant;
		static const LogId Dequant;
		static const LogId LumaPrediction;
		static const LogId LumaReco;
		static const LogId ChromaPred;
		static const LogId ChromaReco;
		static const LogId DeblockingFilter;
		static const LogId DeblockingFilterResult;
		static const LogId RDO;
		static const LogId Binarization;
		static const LogId BinarizationSummary;
		static const LogId Prediction;
		static const LogId Overview;
		static const LogId BinOut;

		Void printSpaces();
		Void increaseSpaces();
		Void decreaseSpaces();
		Void setTabLength(Int len);
		UInt getTabLength();
		Void setTabStep(UInt len);

		std::ostream& getStream();
		template<typename T>
		Void printValues(T&& arg);
		template<typename T1, typename... Tn>
		Void printValues(T1&& arg1, Tn&&... args);
	};

	class LoggingControl : public Singleton < LoggingControl >
	{
	private:

		std::vector<std::string> _logNames;
		std::map<LogId, std::shared_ptr<Logger>> _swapBuffer;
		std::map<LogId, std::shared_ptr<Logger>> _logs;
		std::map<LogId, UInt> _muteStack;

		Void loadSettings();
		std::shared_ptr<Logger> createLog(LogId logId, std::string path);

	public:

		LoggingControl();
		~LoggingControl();

		static std::string mainSettingsPath;

		std::shared_ptr<Logger> getLog(LogId logId);

		void turnOff(LogId logId);
		void turnOn(LogId logId);

		template<typename... Params>
		Void printlnToLog(LogId id, Params... args);
		template<typename... Params>
		Void printToLog(LogId id, Params... args);
		template<typename T>
		Void printArrayToLog(LogId id, const Char* name, T** matrix, size_t sizeX, size_t sizeY);
		template<typename T>
		Void printArrayToLog1Dto2D(LogId id, const Char* name, T* matrix, size_t sizeX, size_t sizeY, size_t stride);
		template<typename T, template<class> class TMatrix>
		void printMatrix(LogId logId, TMatrix<T>& matrix);
		template<typename T, template<class> class TMatrix, template<class> class SptrTMatrix>
		void printMatrix(LogId logId, SptrTMatrix<TMatrix<T>> pmatrix);
	};

#pragma region Impl

	template<typename... Params>
	Void LoggingControl::printlnToLog(LogId id, Params... args)
	{
		auto log = instance()._logs[id];
		if (log == nullptr)
			return;
		log->printSpaces();
		log->printValues(args...);
		log->getStream() << std::endl;
	}

	template<typename... Params>
	Void LoggingControl::printToLog(LogId id, Params... args)
	{
		auto log = instance()._logs[id];
		if (log == nullptr)
			return;
		log->printSpaces();
		log->printValues(args...);
	}

	template<typename T>
	Void LoggingControl::printArrayToLog(LogId id, const Char* name, T** matrix, size_t sizeX, size_t sizeY)
	{
		auto log = instance()._logs[id];
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
	Void LoggingControl::printArrayToLog1Dto2D(LogId id, const Char* name, T* matrix, size_t sizeX, size_t sizeY, size_t stride)
	{
		auto log = instance()._logs[id];
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
	void LoggingControl::printMatrix(LogId logId, TMatrix<T>& matrix)
	{
		auto log = instance()._logs[logId];
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
	void LoggingControl::printMatrix(LogId logId, SptrTMatrix<TMatrix<T>> pmatrix)
	{
		auto log = instance()._logs[logId];
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
	inline Void Logger::write(NewLineToken)
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

#define STRINGIZE(x) #x 
#define PRINTVAR(x) (#x),": ",(x),NewLineToken()
#define BINARIZE(x,numBits) (#x),": ",std::bitset<numBits>(x),NewLineToken()

#define GET_MACRO(_1,_2,_3,_4,NAME,...) NAME

#define COMBINE1(a,b) a##b
#define COMBINE(a,b) COMBINE1(a,b)

#ifdef _DEBUG

#define LOGLN(...) LoggingControl::instance().printlnToLog(__VA_ARGS__)
#define LOG(...) LoggingControl::instance().printToLog(__VA_ARGS__)
#define LOG_OFF(logId) LoggingControl::instance().turnOff(logId)
#define LOG_ON(logId) LoggingControl::instance().turnOn(logId)
#define LOG_ARRAY_SQUARE(logId,x,mat_size) LoggingControl::printArrayToLog(logId,(#x),(x),mat_size,mat_size)
#define LOG_ARRAY_RECT(logId,x,mat_sizeX, mat_sizeY) LoggingControl::printArrayToLog(logId,(#x),(x),mat_sizeX,mat_sizeY)
#define LOG_ARRAY_1D(logId,x,mat_sizeX, mat_sizeY, stride) LoggingControl::printArrayToLog1Dto2D(logId,(#x),(x),mat_sizeX,mat_sizeY, stride)
#define LOG_ARRAY(...) GET_MACRO(__VA_ARGS__, LOG_ARRAY_RECT, LOG_ARRAY_SQUARE)(__VA_ARGS__)

#define LOG_SCOPE_INDENT(logId,title) Indent COMBINE(logIndent,__LINE__)(title,logId)
#define LOG_FUNCTION_INDENT(logId) Indent COMBINE(logIndent,__LINE__)(__FUNCTION__,logId)
#define LOG_SCOPE_MUTE(logId) Mute COMBINE(logMute,__LINE__)(logId)
#define LOG_MATRIX(log,matrix) LoggingControl::printMatrix(log, matrix)

#else

#define LOGLN(...)
#define LOG(...)
#define LOG_OFF(logId)
#define LOG_ON(logId)
#define LOG_MATRIX(logId,x,mat_size)
#define LOG_MATRIX(logId,x,mat_sizeX, mat_sizeY)
#define LOG_ARRAY_SQUARE(logId,x,mat_size)
#define LOG_ARRAY_RECT(logId,x,mat_sizeX, mat_sizeY)
#define LOG_ARRAY_1D(logId,x,mat_sizeX, mat_sizeY, stride)
#define LOG_ARRAY(...)

#define LOG_SCOPE_INDENT(logId,title)
#define LOG_FUNCTION_INDENT(logId)
#define LOG_SCOPE_MUTE(logId)
#define LOG_MATRIX(log,matrix)

#endif

	class Indent
	{
	private:

		LogId _logId;

		Void tab(LogId logId);
		Void untab(LogId logId);

	public:

		Indent(LogId inId);
		Indent(const Char* func, LogId inId);

		~Indent();
	};

	class Mute
	{
	private:

		LogId _logId;

	public:

		Mute(LogId inId);
		~Mute();
	};
}
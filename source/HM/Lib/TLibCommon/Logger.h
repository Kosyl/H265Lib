#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <string>

namespace HEVC
{
	class Bitset : public std::vector<bool>
	{
	public:
		Bitset(size_t size):
			std::vector<bool>(size)
		{

		}

		template<typename T>
		Bitset(size_t size, T value) :
			std::vector<bool>(size)
		{
			for (size_t i = 0; i < size; ++i)
			{
#pragma warning( push )
#pragma warning( disable : 4800 )
				auto bit = value & (1 << (size - i - 1));
				this->at(i) = bit;
#pragma warning( pop )
			}
		}

		Bitset() :
			vector<bool>(0)
		{

		}

		std::string str() 
		{
			std::string s;
			for (size_t i = 0; i < size(); ++i)
			{
				s += (*this).at(i) ? "1" : "0";
			}
			return s;
		}
	};

	template<typename T>
	class Singleton
	{
	protected:
		Singleton()
		{

		};
		virtual ~Singleton()
		{

		}

	public:
		static T& instance()
		{
			static T _instance;
			return _instance;
		}
	};

	using LogId = std::string;

	struct NewLineToken
	{

	};

	class Logger
	{
	private:

		std::string _logPath;
		std::ostream* _logStream;

		std::string _spaces;
		int _numTabs;
		int _step;

		bool _shouldDeleteStream;

		template<typename T>
		void write(T s);
		template<>
		void write(NewLineToken);

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
		static const LogId IO;
		static const LogId BinOut;
    static const LogId Encoder;

		static const LogId Decoder;

		void printSpaces();
		void increaseSpaces();
		void decreaseSpaces();
		void setTabLength(int len);
		int getTabLength();
		void setTabStep(int len);

		std::ostream& getStream();
		template<typename T>
		void printValues(T&& arg);
		template<typename T1, typename... Tn>
		void printValues(T1&& arg1, Tn&&... args);
	};

	class LoggingControl : public Singleton < LoggingControl >
	{
	private:

		std::vector<std::string> _logNames;
		std::map<LogId, std::shared_ptr<Logger>> _swapBuffer;
		std::map<LogId, std::shared_ptr<Logger>> _logs;
		std::map<LogId, int> _muteStack;

		void loadSettings();
		std::shared_ptr<Logger> createLog(LogId logId, std::string path);

	public:

		LoggingControl();
		~LoggingControl();

		static std::string mainSettingsPath;

		std::shared_ptr<Logger> getLog(LogId logId);

		void turnOff(LogId logId);
		void turnOn(LogId logId);

		template<typename... Params>
		void printlnToLog(LogId id, Params... args);
		template<typename... Params>
		void printToLog(LogId id, Params... args);
		template<typename T>
		void printArrayToLog(LogId id, const char* name, T** matrix, size_t sizeX, size_t sizeY);
		template<typename T>
		void printArrayToLog1Dto2D(LogId id, const char* name, T* matrix, size_t sizeX, size_t sizeY, size_t stride);
		template<typename T, template<class> class TMatrix>
		void printMatrix(LogId logId, TMatrix<T>& matrix);
		template<typename T, template<class> class TMatrix, template<class> class SptrTMatrix>
		void printMatrix(LogId logId, SptrTMatrix<TMatrix<T>> pmatrix);
	};

#pragma region Impl

	template<typename... Params>
	void LoggingControl::printlnToLog(LogId id, Params... args)
	{
		auto log = instance()._logs[id];
		if (log == nullptr)
			return;
		log->printSpaces();
		log->printValues(args...);
		log->getStream() << std::endl;
	}

	template<typename... Params>
	void LoggingControl::printToLog(LogId id, Params... args)
	{
		auto log = instance()._logs[id];
		if (log == nullptr)
			return;
		log->printSpaces();
		log->printValues(args...);
	}

	template<typename T>
	void LoggingControl::printArrayToLog(LogId id, const char* name, T** matrix, size_t sizeX, size_t sizeY)
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
	void LoggingControl::printArrayToLog1Dto2D(LogId id, const char* name, T* matrix, size_t sizeX, size_t sizeY, size_t stride)
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
		for (size_t y = 0; y < matrix.height(); ++y)
		{
			log->printSpaces();
			for (size_t x = 0; x < matrix.width(); ++x)
			{
				log->printValues(matrix(x, y), " ");
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
		for (size_t y = 0; y < pmatrix->height(); ++y)
		{
			log->printSpaces();
			for (size_t x = 0; x < pmatrix->width(); ++x)
			{
				log->printValues(pmatrix->at(x, y), " ");
			}
			log->getStream() << std::endl;
		}
	}

	template<typename T>
	void Logger::write(T s)
	{
		*_logStream << s;
	}

	template<>
	inline void Logger::write(NewLineToken)
	{
		*_logStream << std::endl;
		printSpaces();
		_logStream->flush();
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
#define PRINTVAR(x) (#x),": ",(x),"; "
#define PRINTLNVAR(x) (#x),": ",(x),NewLineToken()
#define BINARIZE(x,numBits) (#x),": ",std::bitset<numBits>(x),NewLineToken()

#define GET_MACRO(_1,_2,_3,_4,NAME,...) NAME

#define COMBINE1(a,b) a##b
#define COMBINE(a,b) COMBINE1(a,b)

#ifdef _DEBUG

#define LOGLN(...) LoggingControl::instance().printlnToLog(__VA_ARGS__)
#define LOG(...) LoggingControl::instance().printToLog(__VA_ARGS__)
#define LOG_OFF(logId) LoggingControl::instance().turnOff(logId)
#define LOG_ON(logId) LoggingControl::instance().turnOn(logId)
#define LOG_ARRAY_SQUARE(logId,x,mat_size) LoggingControl::instance().printArrayToLog(logId,(#x),(x),mat_size,mat_size)
#define LOG_ARRAY_RECT(logId,x,mat_sizeX, mat_sizeY) LoggingControl::instance().printArrayToLog(logId,(#x),(x),mat_sizeX,mat_sizeY)
#define LOG_ARRAY_1D(logId,x,mat_sizeX, mat_sizeY, stride) LoggingControl::instance().printArrayToLog1Dto2D(logId,(#x),(x),mat_sizeX,mat_sizeY, stride)
#define LOG_ARRAY(...) GET_MACRO(__VA_ARGS__, LOG_ARRAY_RECT, LOG_ARRAY_SQUARE)(__VA_ARGS__)
#define LOG_MATRIX(log,matrix) LoggingControl::instance().printMatrix(log, matrix)

#define LOG_SCOPE_INDENT(logId,title) Indent COMBINE(logIndent,__LINE__)(title,logId)
#define LOG_FUNCTION_INDENT(logId) Indent COMBINE(logIndent,__LINE__)(__FUNCTION__,logId)
#define LOG_SCOPE_MUTE(logId) Mute COMBINE(logMute,__LINE__)(logId)

#define LOG_JSON_SCOPE(logId) Indent COMBINE(logIndent,__LINE__)(logId,"{},")
#define LOG_JSON_NAMED_SCOPE(logId,name)	LOGLN(logId,(#name), ": "); \
																					Indent COMBINE(logIndent,__LINE__)(logId,"{},")

#define LOG_JSON_ARRAY_SCOPE(logId,name)	LOGLN(logId,(#name), ": "); \
																					Indent COMBINE(logIndent,__LINE__)(logId,"[],")
#define PRINTVAR_JSON(x) "\"",(#x), "\" : ",(x),NewLineToken()
#define LOGLN_JSON(logId, name, value) LoggingControl::instance().printlnToLog(logId,(#name),": ",value,",");

#else

#define LOGLN(...) 
#define LOG(...)
#define LOG_OFF(logId)
#define LOG_ON(logId)
#define LOG_ARRAY_SQUARE(logId,x,mat_size) 
#define LOG_ARRAY_RECT(logId,x,mat_sizeX, mat_sizeY)
#define LOG_ARRAY_1D(logId,x,mat_sizeX, mat_sizeY, stride)
#define LOG_ARRAY(...) 
#define LOG_MATRIX(log,matrix)

#define LOG_SCOPE_INDENT(logId,title) 
#define LOG_FUNCTION_INDENT(logId) 
#define LOG_SCOPE_MUTE(logId)

#endif

	class Indent
	{
	private:

		LogId _logId;
		std::string delimeters{ "{}" };

		void tab(LogId logId);
		void untab(LogId logId);

	public:

		Indent(LogId inId);
		Indent( LogId inId, std::string custom_delimeters );
		Indent(const char* func, LogId inId);

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
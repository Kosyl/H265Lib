
#ifndef _LOGGER_
#define _LOGGER_

/**
* @file	Logger.h
*
* @brief	Deklaracje klas logowania
*/


#include "TypeDef.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <vector>
#include <map>

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

#define STRINGIZE(x) #x 
#define PRINTVAR(x) (#x),": ",(x)," "

enum Logs
{
	ForwardTransform = 0,//1,
	InverseTransform = 0,//2,
	Quant = 0,//3,
	Dequant = 0,//4,
	LumaPrediction = 0,//6
	LumaReco = 0,//6,
	ChromaPred = 0,//7,
	ChromaReco = 0,//8,
	DeblockingFilter = 0,//9,
	DeblockingFilterResult = 0,//10,
	RDO = 11,
	Binarization = 0,//12,
	BinarizationSummary = 0,//13,
	Prediction = 0//14
};



/**
* @class	Logger
*
* @brief	A logger.
*/

class Logger
{
private:

	/**
	* @brief	Sciezka do pliku logu.
	*/

	std::string m_logPath;

	/**
	* @brief	Strumien do pliku logu.
	*/

	std::ofstream m_logStream;

	std::string m_spaces;
	/**
	* @brief	Iloœæ tabulacji
	*/

	UInt m_numTabs;

	/**
	* @brief	Iloœæ spacji/tabulacja.
	*/

	UInt m_step;

	/**
	* @fn	Void Logger::printSpaces( );
	*
	* @brief	Wypisuje aktualn¹ liczbê tabulacji.
	*
	* @return	Void.
	*/

	/**
	* @fn	template<typename T> Void Logger::write( T s, identity<T> i );
	*
	* @brief	Ogólna funkcja zapisuj¹ca:
	*
	* @tparam	T	Obiekt do zapisu do strumienia.
	* @param	s	Obiekt typu T do zapisu do strumienia.
	* @param	i	pomocnicza struktura-klucz.
	*
	* @return	Void.
	*/

	template<typename T>
	Void write(T s)
	{
		m_logStream << s;
	}

public:

	Void printSpaces()
	{
		m_logStream << m_spaces;
	}

	/**
	* @fn	Logger::Logger( std::string logPath, Bool isLogging );
	*
	* @brief	Konstruktor.
	*
	* @param	logPath	 	Pe³na œcie¿ka do pliku logowania
	* @param	isLogging	W³¹czenie/wy³¹czenie
	*/

	Logger(std::string logPath) :
		m_logPath(logPath),
		m_numTabs(0),
		m_step(2),
		m_spaces("")
	{
		m_logStream.open(logPath, std::fstream::out | std::fstream::ate);
	}

	/**
	* @fn	Logger::~Logger( );
	*
	* @brief	Destruktor.
	*/

	~Logger()
	{
		if (m_logStream.is_open())
		{
			m_logStream.flush();
			m_logStream.close();
		}
	}

	/**
	* @fn	Void Logger::increaseSpaces( );
	*
	* @brief	Wciêcie w logowaniu.
	*
	* @return	Void.
	*/

	Void increaseSpaces()
	{
		setTabLength(m_numTabs + 1);
	}

	/**
	* @fn	Void Logger::decreaseSpaces( );
	*
	* @brief	Decrease spaces.
	*
	* @return	Void.
	*/

	Void decreaseSpaces()
	{
		setTabLength(m_numTabs - 1);
	}

	/**
	* @fn	Void Logger::setTabLength( UInt len );
	*
	* @brief	Ustawienie ilosci tabulacji.
	*
	* @param	len	Wybrana liczba tabulacji.
	*
	* @return	Void.
	*/

	Void setTabLength(UInt len)
	{
		if (len >= 0)
		{
			m_spaces = "";
			for (UInt i = 0; i < len*m_step; ++i)
			{
				m_spaces = m_spaces.append(" ");
			}

			m_numTabs = len;
		}
		else
			setTabLength(0);
	}

	/**
	* @fn	Void Logger::setTabStep( UInt len );
	*
	* @brief	Ustawienie d³ugoœci tabulacji.
	*
	* @param	len	Wybrana d³ugoœæ tabulacji (w spacjach).
	*
	* @return	Void.
	*/

	Void setTabStep(UInt len)
	{
		m_step = len;
		setTabLength(m_numTabs);
	}

	/**
	* @fn	std::ofstream& Logger::getStream( );
	*
	* @brief	Dostêp do strumienia wyjœciowego.
	*
	* @return	Strumieñ wyjœciowy.
	*/

	std::ofstream& getStream()
	{
		return this->m_logStream;
	}

	/**
	* @fn	template<typename T> Void Logger::write( T s );
	*
	* @brief	Ogólna funkcja zapisu do pliku.
	*
	* @tparam	T	Dowolna klasa/typ, które mog¹ zostaæ podane na strumieñ.
	* @param	s	instancja T to zapisania
	*
	* @return	Void.
	*/

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

};

/**
* @class	LoggingControl
*
* @brief	A logging control.
*/

class LoggingControl
{
private:

	/**
	* @brief	Instancja.
	*/

	static LoggingControl *instance;

	/**
	* @brief	Lista nazw logów.
	*/

	std::vector<std::string> logNames;

	/**
	* @fn	LoggingControl::LoggingControl( );
	*
	* @brief	Konstruktor domyœlny.
	*/

	LoggingControl();

	/**
	* @fn	void LoggingControl::LoadSettings( );
	*
	* @brief	Wczytanie konfiguracji z pliku.
	*/

	void LoadSettings();

public:

	/**
	* @brief	Œcie¿ka do pliku z konfigutacj¹ optogramowania
	*/

	static std::string mainSettingsPath;

	/**
	* @fn	static LoggingControl* LoggingControl::getInstance( );
	*
	* @brief	Dostêp do instancji kontroli logowania.
	*
	* @return	WskaŸnik na instancjê HMLoggingControl
	*/

	static LoggingControl* getInstance();

	/**
	* @fn	LoggingControl::~LoggingControl( );
	*
	* @brief	Destruktor.
	*/

	~LoggingControl();

	/**
	* @brief	Mapa Loggerów.
	*/

	std::map<Logs, Logger*> logs;
};

/**
* @fn	Void LOG_TAB( std::string key = "" );
*
* @brief	Robi wciêcie w pliku odpowiadajacemu podanemu kluczowi.
*
* @param	key	Klucz obiektu logu.
*
* @return	Void.
*/

Void LOG_TAB(Logs key);

/**
* @fn	Void LOG_UNTAB( std::string key = "" );
*
* @brief	Usuwa jedno wciêcie w pliku z danego klucza.
*
* @param	key	Klucz obiektu logu.
*
* @return	Void.
*/

Void LOG_UNTAB(Logs key);

template<typename... Params>
Void printToLog(Logs id, Params... args)
{
	Logger* log = LoggingControl::getInstance()->logs[id];
	if (log == nullptr)
		return;
	log->printSpaces();
	log->printValues(args...);
	log->getStream() << std::endl;
}
template<typename T>
Void printMatrixToLog(Logs id, const char* name, T** matrix, int sizeX, int sizeY)
{
	Logger* log = LoggingControl::getInstance()->logs[id];
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
Void printMatrixToLog1Dto2D(Logs id, const char* name, T* matrix, int sizeX, int sizeY, int stride)
{
	Logger* log = LoggingControl::getInstance()->logs[id];
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
			log->printValues(matrix[i*stride+j], " ");
		}
		log->getStream() << std::endl;
	}
}

#ifdef _DEBUG
#define LOG(...) printToLog(__VA_ARGS__)

#define GET_MACRO(_1,_2,_3,_4,NAME,...) NAME
#define LOG_MATRIX_SQUARE(logId,x,mat_size) printMatrixToLog(logId,(#x),(x),mat_size,mat_size)
#define LOG_MATRIX_RECT(logId,x,mat_sizeX, mat_sizeY) printMatrixToLog(logId,(#x),(x),mat_sizeX,mat_sizeY)
#define LOG_MATRIX_1D(logId,x,mat_sizeX, mat_sizeY, stride) printMatrixToLog1Dto2D(logId,(#x),(x),mat_sizeX,mat_sizeY, stride)
#define LOG_MATRIX(...) GET_MACRO(__VA_ARGS__, LOG_MATRIX_RECT, LOG_MATRIX_SQUARE)(__VA_ARGS__)

#define COMBINE1(a,b) a##b
#define COMBINE(a,b) COMBINE1(a,b)
#define LOG_SCOPE_INDENT(logId) Indent indent##logId(logId)
#define LOG_FUNCTION_INDENT(logId) Indent COMBINE(indent,__LINE__)(__FUNCTION__,logId)
#else
#define LOG(...)
#define LOG_MATRIX(logId,x,mat_size) 
#define LOG_MATRIX(logId,x,mat_sizeX, mat_sizeY)
#endif

class Indent
{
	Logs m_id;
public:
	Indent(Logs inId):
		m_id(inId)
	{
		LOG_TAB(inId);
	}
	Indent(const char* func, Logs inId) :
		m_id(inId)
	{
		LOG(inId, func);
		LOG_TAB(inId);
	}
	~Indent()
	{
		LOG_UNTAB(m_id);
	}
};

#endif
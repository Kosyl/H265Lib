/**
 * @file	Logger.cpp
 *
 * @brief	Implementacja klas loguj¹cych
 */

#include "Logger.h"

Logger::Logger()
{
	Logger("D:\\txt\\default_log.txt", false);
}

/** Konstruktor przyjmuje sciezke do logu i rozkaz wlaczenia/nie wlaczania logu
*	\param logPath sciezka do pliku logu
\param isLogging pisze log, jesli true
*/
Logger::Logger(std::string logPath, Bool isLogging) :
includeDetails(true),
m_loggingOn(isLogging),
m_printSpaces(true),
m_logPath(logPath),
m_numTabs(0),
m_step(2),
m_spaces("")
{
	if (isLogging)
	{
		m_logStream.open(logPath, std::fstream::out | std::fstream::ate);
	}
}

Logger::~Logger()
{
	if (m_logStream.is_open())
	{
		m_logStream.flush();
		m_logStream.close();
	}
}

/** Wlaczenie/wylaczenie logowania
*	\param log true, jesli ma logowac; false, jesli nie
*	\return void
*/

Void Logger::printSpaces()
{
	m_printSpaces = false;
	m_logStream << m_spaces;
}

void Logger::setLogging(Bool log)
{
	if (log)
	{
		if (!m_logStream.is_open())
		{
			m_logStream.open(m_logPath, std::fstream::out | std::fstream::ate);
		}
	}
	else
	{
		m_logStream.flush();
		m_logStream.close();
	}
}

Void Logger::changeLogPath(std::string path)
{
	if (m_logStream.is_open())
	{
		m_logStream.flush();
		m_logStream.close();
	}

	m_logPath = path;
	m_logStream.open(path, std::fstream::out | std::fstream::ate);

	return;
}

Void Logger::setTabLength(UInt len)
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

Void Logger::setTabStep(UInt len)
{
	m_step = len;
	setTabLength(m_numTabs);
}

Void Logger::increaseSpaces()
{
	setTabLength(m_numTabs + 1);
}

Void Logger::decreaseSpaces()
{
	setTabLength(m_numTabs - 1);
}

Logger& operator<<(Logger& o, std::ostream& (*f)(std::ostream&))
{
		if (o.isLogging())
		{
			o.write("\n");
		}
	return o;
}

std::ofstream& Logger::getStream()
{
	return this->m_logStream;
}

//////////////////////////////////////

LoggingControl* LoggingControl::instance = nullptr;

LoggingControl::LoggingControl()
{
	this->LoadSettings();
}

LoggingControl* LoggingControl::getInstance()
{
	if (LoggingControl::instance == nullptr)
		LoggingControl::instance = new LoggingControl();
	return LoggingControl::instance;
}

LoggingControl::~LoggingControl()
{
	delete logs[LOGGER_FORWARD_TRANSFORM];
	delete logs[LOGGER_INVERSE_TRANSFORM];
	delete logs[LOGGER_QUANTIZATION];
	delete logs[LOGGER_DEQUANTIZATION];
	delete logs[LOGGER_LUMA_PREDICTION];
	delete logs[LOGGER_LUMA_RECONSTRUCTION];
	delete logs[LOGGER_CHROMA_PREDICTION];
	delete logs[LOGGER_CHROMA_RECONSTRUCTION];
	delete logs[LOGGER_RDO];
	delete logs[LOGGER_BINARIZATION];
	delete logs[LOGGER_BINARIZATION_SUMMARY];
	delete logs[LOGGER_DEBLOCKING_FILTER];
	delete logs[LOGGER_DEBLOCKING_FILTER_RESULT];
	delete logs[LOGGER_PREDICTION];
}

void LoggingControl::LoadSettings()
{
	logs[LOGGER_FORWARD_TRANSFORM] = new Logger(LOGPATH_FT);
	logs[LOGGER_INVERSE_TRANSFORM] = new Logger(LOGPATH_IT);
	logs[LOGGER_QUANTIZATION] = new Logger(LOGPATH_Q);
	logs[LOGGER_DEQUANTIZATION] = new Logger(LOGPATH_IQ);
	logs[LOGGER_LUMA_PREDICTION] = new Logger(LOGPATH_LP);
	logs[LOGGER_LUMA_RECONSTRUCTION] = new Logger(LOGPATH_LR);
	logs[LOGGER_CHROMA_PREDICTION] = new Logger(LOGPATH_CP);
	logs[LOGGER_CHROMA_RECONSTRUCTION] = new Logger(LOGPATH_CR);
	logs[LOGGER_RDO] = new Logger(LOGPATH_RDO);
	logs[LOGGER_BINARIZATION] = new Logger(LOGPATH_EC);
	logs[LOGGER_BINARIZATION_SUMMARY] = new Logger(LOGPATH_EC_SUMMARY);
	logs[LOGGER_DEBLOCKING_FILTER] = new Logger(LOGPATH_FDB);
	logs[LOGGER_DEBLOCKING_FILTER_RESULT] = new Logger(LOGPATH_FDBRES);
	logs[LOGGER_PREDICTION] = new Logger(LOGPATH_PRED);
}

Logger& LOG(LogId key)
{
	return *(LoggingControl::getInstance()->logs[key]);
}

Void LOG_TAB(LogId key)
{
	LOG(key) << "{" << std::endl;
	LoggingControl::getInstance()->logs[key]->increaseSpaces();
}

Void LOG_UNTAB(LogId key)
{
	LoggingControl::getInstance()->logs[key]->decreaseSpaces();
	LOG(key) << "}" << std::endl;
}
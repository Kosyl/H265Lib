
#include "Logger.h"

namespace H265Lib
{
	LoggingControl::LoggingControl()
	{
		this->LoadSettings();
	}

	void LoggingControl::turnOff(Logs logId)
	{
		muteStack[logId]++;

		if (muteStack[logId] > 0 && logs[logId] != nullptr)
		{
			swapBuffer[logId] = logs[logId];
			logs[logId] = nullptr;
		}
	}

	void LoggingControl::turnOn(Logs logId)
	{
		if (muteStack[logId] > 0)
		{
			muteStack[logId]--;
			if (muteStack[logId] == 0 && swapBuffer[logId] != nullptr)
			{
				logs[logId] = swapBuffer[logId];
				swapBuffer[logId] = nullptr;
			}
		}
	}

	LoggingControl::~LoggingControl()
	{
	}

	void LoggingControl::LoadSettings()
	{

		logs[Logs::Console] = createLog(Logs::Console, "");
		logs[Logs::Dump] = createLog(Logs::Dump, "");

		logs[Logs::ForwardTransform] = createLog(Logs::ForwardTransform, LOGPATH_FT);
		logs[Logs::InverseTransform] = createLog(Logs::InverseTransform,LOGPATH_IT);
		logs[Logs::Quant] = createLog(Logs::Quant,LOGPATH_Q);
		logs[Logs::Dequant] = createLog(Logs::Dequant,LOGPATH_IQ);
		logs[Logs::LumaPrediction] = createLog(Logs::LumaPrediction,LOGPATH_LP);
		logs[Logs::LumaReco] = createLog(Logs::LumaReco,LOGPATH_LR);
		logs[Logs::ChromaPred] = createLog(Logs::ChromaPred,LOGPATH_CP);
		logs[Logs::ChromaReco] = createLog(Logs::ChromaReco,LOGPATH_CR);
		logs[Logs::RDO] = createLog(Logs::RDO,LOGPATH_RDO);
		logs[Logs::Binarization] = createLog(Logs::Binarization,LOGPATH_EC);
		logs[Logs::BinarizationSummary] = createLog(Logs::BinarizationSummary,LOGPATH_EC_SUMMARY);
		logs[Logs::DeblockingFilter] = createLog(Logs::DeblockingFilter,LOGPATH_FDB);
		logs[Logs::DeblockingFilterResult] = createLog(Logs::DeblockingFilterResult,LOGPATH_FDBRES);
		logs[Logs::Prediction] = createLog(Logs::Prediction,LOGPATH_PRED);
		logs[Logs::Overview] = createLog(Logs::Overview,LOGPATH_OVERVIEW);
		logs[Logs::BinOut] = createLog(Logs::BinOut,LOGPATH_BINOUT);
	}

	std::shared_ptr<Logger> LoggingControl::createLog(Logs logId, std::string path)
	{
		if (logId == Logs::Off)
			return nullptr;
		if (logId == Logs::Console)
			return Logger::createConsoleLogger();
		if (logId == Logs::Dump)
			return Logger::createFileLogger("D:\\dump.txt");
		return Logger::createFileLogger(path);
	}

	Void Indent::tab(Logs key)
	{
		auto log = LoggingControl::instance().logs[key];
		if (log == nullptr)
			return;
		log->printSpaces();
		log->getStream() << "{" << std::endl;
		log->increaseSpaces();
	}

	Void Indent::untab(Logs key)
	{
		auto log = LoggingControl::instance().logs[key];
		if (log == nullptr)
			return;
		log->decreaseSpaces();
		log->printSpaces();
		log->getStream() << "}" << std::endl;
	}

	Indent::Indent(Logs inId):
		m_id(inId)
	{
		auto log = LoggingControl::instance().logs[inId];
		if (log == nullptr)
			return;
		tab(inId);
	}

	Indent::Indent(const char* func, Logs inId):
		m_id(inId)
	{
		auto log = LoggingControl::instance().logs[inId];
		if (log == nullptr)
			return;
		LOGLN(inId, func);
		tab(inId);
	}

	Indent::~Indent()
	{
		auto log = LoggingControl::instance().logs[m_id];
		if (log == nullptr)
			return;
		untab(m_id);
	}

	Mute::Mute(Logs inId):
		m_id(inId)
	{
		LOG_OFF(inId);
	}

	Mute::~Mute()
	{
		LOG_ON(m_id);
	}

	Logger::Logger(std::string logPath):
		_logPath(logPath),
		_numTabs(0),
		_step(2),
		_spaces(""),
		_shouldDeleteStream(true)
	{
		_logStream = new std::ofstream(logPath, std::fstream::out | std::fstream::ate);
	}

	Logger::Logger():
		_logPath(""),
		_numTabs(0),
		_step(2),
		_spaces(""),
		_shouldDeleteStream(false)
	{
		_logStream = &std::cout;
	}

	std::shared_ptr<Logger> Logger::createFileLogger(std::string logPath)
	{
		return std::make_shared<Logger>(logPath);
	}

	std::shared_ptr<Logger> Logger::createConsoleLogger()
	{
		return std::make_shared<Logger>();
	}

	Void Logger::printSpaces()
	{
		for (UInt i = 0; i < _numTabs; ++i)
			*_logStream << "  ";
	}

	Logger::~Logger()
	{
		if (_shouldDeleteStream)
		{
			_logStream->flush();
			delete _logStream;
		}
		_logStream = nullptr;
	}

	Void Logger::increaseSpaces()
	{
		setTabLength(_numTabs + 1);
	}

	Void Logger::decreaseSpaces()
	{
		setTabLength(_numTabs - 1);
	}

	Void Logger::setTabLength(Int len)
	{
		if (len >= 0)
		{
			_numTabs = len;
		}
		else
			setTabLength(0);
	}

	UInt Logger::getTabLength()
	{
		return _numTabs;
	}

	Void Logger::setTabStep(UInt len)
	{
		_step = len;
		setTabLength(_numTabs);
	}

	std::ostream& Logger::getStream()
	{
		return *_logStream;
	}
}
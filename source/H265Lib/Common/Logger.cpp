#include "Logger.h"
#include <fstream>

namespace HEVC
{
	const LogId Logger::Off{ "Off" };
	const LogId Logger::Console{ "Console" };
	const LogId Logger::Dump{ "Dump" };
	const LogId Logger::ForwardTransform{ "ForwardTransform" };
	const LogId Logger::InverseTransform{ "InverseTransform" };
	const LogId Logger::Quant{ "Quant" };
	const LogId Logger::Dequant{ "Dequant" };
	const LogId Logger::LumaPrediction{ "LumaPrediction" };
	const LogId Logger::LumaReco{ "LumaReco" };
	const LogId Logger::ChromaPred{ "ChromaPred" };
	const LogId Logger::ChromaReco{ "ChromaReco" };
	const LogId Logger::DeblockingFilter{ "DeblockingFilter" };
	const LogId Logger::DeblockingFilterResult{ "DeblockingFilterResult" };
	const LogId Logger::RDO{ "RDO" };
	const LogId Logger::Binarization{ "Binarization" };
	const LogId Logger::BinarizationSummary{ "BinarizationSummary" };
	const LogId Logger::Prediction{ "Prediction" };
	const LogId Logger::Overview{ "Overview" };
	const LogId Logger::BinOut{ "BinOut" };
  const LogId Logger::Encoder{ "Encoder" };

	LoggingControl::LoggingControl()
	{
		this->loadSettings();
	}

	std::shared_ptr<Logger> LoggingControl::getLog(LogId logId)
	{
		return LoggingControl::instance()._logs[logId];
	}

	void LoggingControl::turnOff(LogId logId)
	{
		_muteStack[logId]++;

		if (_muteStack[logId] > 0 && _logs[logId] != nullptr)
		{
			_swapBuffer[logId] = _logs[logId];
			_logs[logId] = nullptr;
		}
	}

	void LoggingControl::turnOn(LogId logId)
	{
		if (_muteStack[logId] > 0)
		{
			_muteStack[logId]--;
			if (_muteStack[logId] == 0 && _swapBuffer[logId] != nullptr)
			{
				_logs[logId] = _swapBuffer[logId];
				_swapBuffer[logId] = nullptr;
			}
		}
	}

	LoggingControl::~LoggingControl()
	{
	}

	void LoggingControl::loadSettings()
	{
		_logs[Logger::Console] = createLog(Logger::Console, "");
		_logs[Logger::Dump] = createLog(Logger::Dump, "");

		_logs[Logger::ForwardTransform] = createLog(Logger::ForwardTransform, "D:\\txt\\ft.txt");
		_logs[Logger::InverseTransform] = createLog(Logger::InverseTransform, "D:\\txt\\it.txt");
		_logs[Logger::Quant] = createLog(Logger::Quant, "D:\\txt\\q.txt");
		_logs[Logger::Dequant] = createLog(Logger::Dequant, "D:\\txt\\iq.txt");
		_logs[Logger::LumaPrediction] = createLog(Logger::LumaPrediction, "D:\\txt\\lp.txt");
		_logs[Logger::LumaReco] = createLog(Logger::LumaReco, "D:\\txt\\lr.txt");
		_logs[Logger::ChromaPred] = createLog(Logger::ChromaPred, "D:\\txt\\cp.txt");
		_logs[Logger::ChromaReco] = createLog(Logger::ChromaReco, "D:\\txt\\cr.txt");
		_logs[Logger::RDO] = createLog(Logger::RDO, "D:\\txt\\rdo.txt");
		_logs[Logger::Binarization] = createLog(Logger::Binarization, "D:\\txt\\ec.txt");
		_logs[Logger::BinarizationSummary] = createLog(Logger::BinarizationSummary, "D:\\txt\\ec_summary.txt");
		_logs[Logger::DeblockingFilter] = createLog(Logger::DeblockingFilter, "D:\\txt\\fdb.txt");
		_logs[Logger::DeblockingFilterResult] = createLog(Logger::DeblockingFilterResult, "D:\\txt\\HMfdbres.txt");
		_logs[Logger::Prediction] = createLog(Logger::Prediction, "D:\\txt\\predykcja.txt");
		_logs[Logger::Overview] = createLog(Logger::Overview, "D:\\txt\\overview.txt");
		_logs[Logger::BinOut] = createLog(Logger::BinOut, "D:\\txt\\binout.txt");
    _logs[Logger::Encoder] = createLog(Logger::Encoder, "D:\\encoder.txt");
	}

	std::shared_ptr<Logger> LoggingControl::createLog(LogId logId, std::string path)
	{
		if (logId == Logger::Off)
			return nullptr;
		if (logId == Logger::Console)
			return Logger::createConsoleLogger();
		if (logId == Logger::Dump)
			return Logger::createFileLogger("D:\\dump.txt");
		return Logger::createFileLogger(path);
	}

	void Indent::tab(LogId key)
	{
		auto log = LoggingControl::instance().getLog(key);
		if (log == nullptr)
			return;
		log->printSpaces();
		log->getStream() << "{" << std::endl;
		log->increaseSpaces();
	}

	void Indent::untab(LogId key)
	{
		auto log = LoggingControl::instance().getLog(key);
		if (log == nullptr)
			return;
		log->decreaseSpaces();
		log->printSpaces();
		log->getStream() << "}" << std::endl;
	}

	Indent::Indent(LogId key) :
		_logId(key)
	{
		auto log = LoggingControl::instance().getLog(key);
		if (log == nullptr)
			return;
		tab(key);
	}

	Indent::Indent(const char* func, LogId key) :
		_logId(key)
	{
		auto log = LoggingControl::instance().getLog(key);
		if (log == nullptr)
			return;
		LOGLN(key, func);
		tab(key);
	}

	Indent::~Indent()
	{
		auto log = LoggingControl::instance().getLog(_logId);
		if (log == nullptr)
			return;
		untab(_logId);
	}

	Mute::Mute(LogId inId) :
		_logId(inId)
	{
		LOG_OFF(inId);
	}

	Mute::~Mute()
	{
		LOG_ON(_logId);
	}

	Logger::Logger(std::string logPath) :
		_logPath(logPath),
		_numTabs(0),
		_step(2),
		_spaces(""),
		_shouldDeleteStream(true)
	{
		_logStream = new std::ofstream(logPath, std::fstream::out | std::fstream::ate);
	}

	Logger::Logger() :
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

	void Logger::printSpaces()
	{
		for (int i = 0; i < _numTabs; ++i)
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

	void Logger::increaseSpaces()
	{
		setTabLength(_numTabs + 1);
	}

	void Logger::decreaseSpaces()
	{
		setTabLength(_numTabs - 1);
	}

	void Logger::setTabLength(int len)
	{
		if (len >= 0)
		{
			_numTabs = len;
		}
		else
			setTabLength(0);
	}

	int Logger::getTabLength()
	{
		return _numTabs;
	}

	void Logger::setTabStep(int len)
	{
		_step = len;
		setTabLength(_numTabs);
	}

	std::ostream& Logger::getStream()
	{
		return *_logStream;
	}
}
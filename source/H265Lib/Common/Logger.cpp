
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

	Void LOG_TAB(Logs key)
	{
		auto log = LoggingControl::instance().logs[key];
		if (log == nullptr)
			return;
		log->printSpaces();
		log->getStream() << "{" << std::endl;
		log->increaseSpaces();
	}

	Void LOG_UNTAB(Logs key)
	{
		auto log = LoggingControl::instance().logs[key];
		if (log == nullptr)
			return;
		log->decreaseSpaces();
		log->printSpaces();
		log->getStream() << "}" << std::endl;
	}
}
/**
* @file	Logger.cpp
*
* @brief	Implementacja klas loguj¹cych
*/

#include "Logger.h"

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
	delete logs[Logs::ForwardTransform];
	delete logs[Logs::InverseTransform];
	delete logs[Logs::Quant];
	delete logs[Logs::Dequant];
	delete logs[Logs::LumaPrediction];
	delete logs[Logs::LumaReco];
	delete logs[Logs::ChromaPred];
	delete logs[Logs::ChromaReco];
	delete logs[Logs::RDO];
	delete logs[Logs::Binarization];
	delete logs[Logs::BinarizationSummary];
	delete logs[Logs::DeblockingFilter];
	delete logs[Logs::DeblockingFilterResult];
	delete logs[Logs::Prediction];
}

void LoggingControl::LoadSettings()
{
	logs[Logs::ForwardTransform] = Logs::ForwardTransform ? new Logger(LOGPATH_FT) : nullptr;
	logs[Logs::InverseTransform] = Logs::InverseTransform ? new Logger(LOGPATH_IT) : nullptr;
	logs[Logs::Quant] = Logs::Quant ? new Logger(LOGPATH_Q) : nullptr;
	logs[Logs::Dequant] = Logs::Dequant ? new Logger(LOGPATH_IQ) : nullptr;
	logs[Logs::LumaPrediction] = Logs::LumaPrediction ? new Logger(LOGPATH_LP) : nullptr;
	logs[Logs::LumaReco] = Logs::LumaReco ? new Logger(LOGPATH_LR) : nullptr;
	logs[Logs::ChromaPred] = Logs::ChromaPred ? new Logger(LOGPATH_CP) : nullptr;
	logs[Logs::ChromaReco] = Logs::ChromaReco ? new Logger(LOGPATH_CR) : nullptr;
	logs[Logs::RDO] = Logs::RDO ? new Logger(LOGPATH_RDO) : nullptr;
	logs[Logs::Binarization] = Logs::Binarization ? new Logger(LOGPATH_EC) : nullptr;
	logs[Logs::BinarizationSummary] = Logs::BinarizationSummary ? new Logger(LOGPATH_EC_SUMMARY) : nullptr;
	logs[Logs::DeblockingFilter] = Logs::DeblockingFilter ? new Logger(LOGPATH_FDB) : nullptr;
	logs[Logs::DeblockingFilterResult] = Logs::DeblockingFilterResult ? new Logger(LOGPATH_FDBRES) : nullptr;
	logs[Logs::Prediction] = Logs::Prediction ? new Logger(LOGPATH_PRED) : nullptr;
}

Void LOG_TAB(Logs key)
{
	Logger* log = LoggingControl::getInstance()->logs[key];
	if (log == nullptr)
		return;
	log->printSpaces();
	log->getStream() << "{" << std::endl;
	log->increaseSpaces();
}

Void LOG_UNTAB(Logs key)
{
	Logger* log = LoggingControl::getInstance()->logs[key];
	if (log == nullptr)
		return;
	log->decreaseSpaces();
	log->printSpaces();
	log->getStream() << "}" << std::endl;
}
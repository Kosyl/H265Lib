#include "LoggingControl.h"
#include <string>
#include <map>

LoggingControl* LoggingControl::instance = nullptr;
std::string LoggingControl::tabs="";
int LoggingControl::tabLen = 0;
std::string LoggingControl::mainSettingsPath = "D:\\h265\\HEVC\\bin\\vc10\\Win32\\Release\\logging.cfg";

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
	if(logNames.size() > 0)
	{
		std::ofstream* osptr=nullptr;
		for(int i=0;i<(logNames.size());++i)
		{
			osptr = logs[logNames[i]];
			if(osptr != nullptr)
			{
				osptr->flush();
				osptr->close();
				delete osptr;
				osptr = nullptr;
			}
		}

		logs.clear();
	}
}

void LoggingControl::LoadSettings()
{
	std::ifstream cfg(LoggingControl::mainSettingsPath);

	std::string s, key, log, value;
	bool isLog = false;

	while (std::getline( cfg, s ))
	{
		std::string::size_type begin = s.find_first_not_of( " \f\t\v" );

		if (begin == std::string::npos) continue;

		std::string::size_type end = s.find( '=', begin );
		key = s.substr( begin, end - begin );

		key.erase( key.find_last_not_of( " \f\t\v" ) + 1 );

		if (key.empty()) continue;

		std::string::size_type dwukropek = s.find( ':', begin );
		log = s.substr( begin, dwukropek - begin );

		log.erase( log.find_last_not_of( " \f\t\v" ) + 1 );
		if (log == "log")
		{
			isLog = true;
			key=s.substr( dwukropek+1, end - dwukropek-1 );
			key.erase( key.find_last_not_of( " \f\t\v" ) + 1 );
		}
		else
			isLog=false;

		begin = s.find_first_not_of( " \f\n\r\t\v", end + 1 );
		end   = s.find_last_not_of(  " \f\n\r\t\v" ) + 1;

		value = s.substr( begin, end - begin );

		if(isLog)
		{
			std::ofstream* stream = new std::ofstream();
			stream->open(value,std::fstream::out | std::fstream::ate);
			logNames.push_back(key);
			logs[key]=stream;
		}
		else
			triggers[key]=value=="1"?true:false;
	}
}

void LoggingControl::setTabsLength(int len)
{
	if(len>=0)
	{
		LoggingControl::tabs = "";
		for(int i = 0; i<len;++i)
		{
			LoggingControl::tabs.append(" ");
		}
		
		LoggingControl::tabLen = len;
	}
	else
		LoggingControl::setTabsLength(0);
}

void LoggingControl::increaseTabsLength(int len)
{
	LoggingControl::setTabsLength(LoggingControl::tabLen + len);
}
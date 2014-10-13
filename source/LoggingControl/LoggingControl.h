#ifndef _LOGGING_CONTROL_
#define _LOGGING_CONTROL_
#include <string>
#include <fstream>
#include <map>
#include <vector>

class LoggingControl
{
private:

	static LoggingControl *instance;
	static std::string mainSettingsPath;
	static int tabLen;

	std::vector<std::string> logNames;

	LoggingControl();

	void LoadSettings();

public:

	static LoggingControl* getInstance();
	~LoggingControl();

	std::map<std::string,std::ofstream*> logs;
	std::map<std::string,bool> triggers;

	static std::string tabs;

	static void setTabsLength(int len);
	static void increaseTabsLength(int len);
	static void decreaseTabsLength(int len);

	int totalBinCount;
	int partialBinCount;
};

#endif
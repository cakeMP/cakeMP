#include <Common.h>
#include <System/Settings.h>

#include <Libs/inih/ini.h>

NAMESPACE_BEGIN;

static int settings_handler(void* user, const char* section, const char* name, const char* value)
{
	Settings &self = *(Settings*)user;

	if (strcmp(section, "Connect") == 0) {
		if (strcmp(name, "Host") == 0) {
			self.ConnectToHost = value;
			logWrite("Settings: Connect to host: %s", self.ConnectToHost.c_str());
			return 1;
		}

		if (strcmp(name, "Port") == 0) {
			self.ConnectToPort = atoi(value);
			logWrite("Settings: Connect to port: %d", self.ConnectToPort);
			return 1;
		}
	}

	return 1;
}

Settings::Settings()
{
	ini_parse(PROJECT_NAME_SHORT ".ini", &settings_handler, this);
}

NAMESPACE_END;

#pragma once

#include <Common.h>

class Settings
{
public:
	std::string ConnectToHost;
	uint16_t ConnectToPort = 4499;

public:
	Settings();
};

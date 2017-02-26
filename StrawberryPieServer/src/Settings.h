#pragma once

#include <Common.h>

class Settings
{
public:
	std::string ListenHost = "0.0.0.0";
	uint16_t ListenPort = 4499;

	uint32_t MaxClients = 100;

	uint32_t TickRate = 60;

	float StreamingRange = 20.0f;

public:
	Settings();
};

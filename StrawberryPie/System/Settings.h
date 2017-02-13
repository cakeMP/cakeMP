#pragma once

#include <Common.h>

NAMESPACE_BEGIN;

class Settings
{
public:
	std::string ConnectToHost = "localhost";
	uint16_t ConnectToPort = 4499;

public:
	Settings();
};

NAMESPACE_END;

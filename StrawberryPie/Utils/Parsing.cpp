#include <Common.h>

#include <Utils/Parsing.h>

NAMESPACE_BEGIN;

std::string prsToString(int32_t x)
{
	char buffer[14];
	sprintf(buffer, "%d", x);
	return buffer;
}

std::string prsToString(uint32_t x)
{
	char buffer[12];
	sprintf(buffer, "%u", x);
	return buffer;
}

std::string prsToString(int64_t x)
{
	char buffer[22];
	sprintf(buffer, "%ll", x);
	return buffer;
}

std::string prsToString(uint64_t x)
{
	char buffer[22];
	sprintf(buffer, "%llu", x);
	return buffer;
}

std::string prsToString(float x)
{
	char buffer[32];
	sprintf(buffer, "%f", x);
	return buffer;
}

int prsToInteger(const char* str)
{
	return atoi(str);
}

int prsToInteger(const std::string &str)
{
	return atoi(str.c_str());
}

float prsToFloat(const char* str)
{
	return atof(str);
}

float prsToFloat(const std::string &str)
{
	return atof(str.c_str());
}

NAMESPACE_END;

#include <Common.h>

#include <Utils/Formatting.h>
#include <Utils/Parsing.h>

NAMESPACE_BEGIN;

std::string fmtString(const char* format, ...)
{
	char stackBuffer[128];

	va_list vl;
	va_start(vl, format);
	int len = vsnprintf(stackBuffer, 128, format, vl);
	va_end(vl);

	if (len < 128) {
		return std::string(stackBuffer);
	}

	char* buffer = (char*)malloc(len + 1);

	va_start(vl, format);
	len = vsnprintf(buffer, len + 1, format, vl);
	va_end(vl);

	std::string ret(buffer);
	free(buffer);

	return ret;
}

std::string fmtBytes(size_t bytes)
{
	if (bytes > 1000 * 1000 * 1000) {
		return prsToString(bytes / 1000 / 1000 / 1000) + "gb";
	} else if (bytes > 1000 * 1000) {
		return prsToString(bytes / 1000 / 1000) + "mb";
	} else if (bytes > 1000) {
		return prsToString(bytes / 1000) + "kb";
	}
	return prsToString(bytes) + "b";
}

NAMESPACE_END;

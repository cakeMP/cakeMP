#include <Common.h>

#include <Utils/Formatting.h>
#include <Utils/Parsing.h>

NAMESPACE_BEGIN;

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

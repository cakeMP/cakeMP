#include <Common.h>

#include <cstdarg>
#include <ctime>

#include <Logging.h>

static std::mutex _mutexLog;

void logWrite(const char* fmt, ...)
{
	_mutexLog.lock();

	time_t tmCur = time(nullptr);
	struct tm* tm = localtime(&tmCur);

	printf(TC_WHITE "[%02d:%02d:%02d] " TC_RESET, tm->tm_hour, tm->tm_min, tm->tm_sec);

	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);

	_mutexLog.unlock();
}

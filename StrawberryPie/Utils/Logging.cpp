#include <Common.h>

#include <Utils/Logging.h>
#include <Utils/Formatting.h>

#include <System/Strawberry.h>

#include <Build.h>

#include <share.h>

NAMESPACE_BEGIN;

static FILE* _fhLog = nullptr;
static std::mutex _mutexLog;

void logOpen(const char* fnm)
{
	if (_fhLog != nullptr) {
		logClose();
	}
	_fhLog = _fsopen(fnm, "ab", _SH_DENYWR);

	time_t tmCur = time(nullptr);
	struct tm* tm = localtime(&tmCur);

	fprintf(_fhLog, "\n    " PROJECT_NAME " starting at %d/%02d/%02d\n\n", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);
	fflush(_fhLog);
}

void logClose()
{
	if (_fhLog == nullptr) {
		return;
	}
	fclose(_fhLog);
	_fhLog = nullptr;
}

void logWrite(const char* fmt, ...)
{
	_mutexLog.lock();

	time_t tmCur = time(nullptr);
	struct tm* tm = localtime(&tmCur);

	if (_fhLog == nullptr) {
		_mutexLog.unlock();
		return;
	}

	char stackBuffer[128];
	std::string lineString;

	va_list args;
	va_start(args, fmt);
	int len = vsnprintf(stackBuffer, 128, fmt, args);
	va_end(args);

	if (len >= 128) {
		char* buffer = (char*)malloc(len + 1);

		va_list args;
		va_start(args, fmt);
		len = vsnprintf(buffer, len + 1, fmt, args);
		va_end(args);

		lineString = buffer;
		free(buffer);
	} else {
		lineString = stackBuffer;
	}

	lineString = fmtString("[%02d:%02d:%02d] %s", tm->tm_hour, tm->tm_min, tm->tm_sec, lineString.c_str());

	fprintf(_fhLog, "%s\n", lineString.c_str());
	fflush(_fhLog);

	if (_pGame != nullptr) {
		_pGame->m_interface.m_chat.AddMessage(lineString);
	}

	_mutexLog.unlock();
}

void logAssertFailed(const char* condition, const char* filename, int line)
{
	logWrite("!! ASSERTION FAILED: '%s'", condition);
	logWrite("   At: %s:%d", filename, line);
}

NAMESPACE_END;

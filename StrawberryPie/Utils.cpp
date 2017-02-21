#include <Common.h>

#include <Utils.h>

#include <share.h>

#include <shv/natives.h>

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

	va_list args;
	va_start(args, fmt);

	fprintf(_fhLog, "[%02d:%02d:%02d] ", tm->tm_hour, tm->tm_min, tm->tm_sec);
	vfprintf(_fhLog, fmt, args);
	fputc('\n', _fhLog);
	fflush(_fhLog);

	va_end(args);

	_mutexLog.unlock();
}

void logAssertFailed(const char* condition, const char* filename, int line)
{
	logWrite("!! ASSERTION FAILED: '%s'", condition);
	logWrite("   At: %s:%d", filename, line);
}

bool mdlRequest(uint32_t hash)
{
	if (STREAMING::HAS_MODEL_LOADED(hash)) {
		return true;
	}

	if (!STREAMING::IS_MODEL_VALID(hash)) {
		logWrite("Model hash %u is invalid!", hash);
		return false;
	}

	STREAMING::REQUEST_MODEL(hash);
	while (!STREAMING::HAS_MODEL_LOADED(hash)) {
		WAIT(0);
	}

	return true;
}

bool mdlRequest(const char* str)
{
	return mdlRequest(hashGet(str));
}

void sndPlayFrontend(const char* name, const char* set)
{
	AUDIO::PLAY_SOUND_FRONTEND(-1, name, set, false);
	AUDIO::RELEASE_SOUND_ID(AUDIO::GET_SOUND_ID());
}

void sndPlayFrontend(const char* name)
{
	AUDIO::PLAY_SOUND_FRONTEND(-1, name, nullptr, false);
	AUDIO::RELEASE_SOUND_ID(AUDIO::GET_SOUND_ID());
}

NAMESPACE_END;

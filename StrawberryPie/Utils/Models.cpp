#include <Common.h>

#include <Utils/Models.h>

#include <shv/natives.h>

NAMESPACE_BEGIN;

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

NAMESPACE_END;

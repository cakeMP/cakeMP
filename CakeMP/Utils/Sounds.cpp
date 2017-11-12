#include <Common.h>

#include <Utils/Sounds.h>

#include <shv/natives.h>

NAMESPACE_BEGIN;

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

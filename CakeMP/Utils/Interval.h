#pragma once

#include <Common.h>

NAMESPACE_BEGIN;

template<int TM>
class Interval
{
public:
	int m_lastTick = 0;

public:
	inline bool CanTick()
	{
		int tmNow = _pGame->m_gameTime;

		if (m_lastTick == 0) {
			m_lastTick = tmNow;
			return true;
		}

		if (m_lastTick - tmNow > TM) {
			m_lastTick = tmNow;
			return true;
		}

		return false;
	}
};

NAMESPACE_END;

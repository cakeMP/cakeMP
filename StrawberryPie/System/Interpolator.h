#pragma once

#include <Common.h>

NAMESPACE_BEGIN;

template<typename T>
class Interpolator
{
private:
	T m_from;
	T m_to;

	float m_factor;

	int m_tmStart;
	int m_tmLength;

public:
	inline void Set(const T &from, const T &to, int ms)
	{
		m_from = from;
		m_to = to;
		m_tmStart = _pGame->m_gameTime;
		m_tmLength = ms;
	}

	inline void Update()
	{
		if (!IsActive()) {
			return;
		}

		int tmNow = _pGame->m_gameTime;
		int tmPassed = tmNow - m_tmStart;
		m_factor = tmPassed / (float)m_tmLength;

		if (m_factor >= 1.0f) {
			m_tmStart = 0;
			m_factor = 1.0f;
		}
	}

	inline T &From() { return m_from; }
	inline T &To() { return m_to; }

	inline T Current() { return glm::mix(m_from, m_to, m_factor); }
	inline T CurrentSpherical() { return glm::slerp(m_from, m_to, m_factor); }

	inline bool IsActive() { return m_tmStart > 0; }
};

NAMESPACE_END;

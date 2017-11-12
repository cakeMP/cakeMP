#pragma once

#include <Common.h>

NAMESPACE_BEGIN;

template<typename T, int NumSamples = 60>
class Average
{
public:
	T m_samples[NumSamples];

public:
	Average()
	{
		for (int i = 0; i < NumSamples; i++) {
			m_samples[i] = 0;
		}
	}

	void Add(const T &sample)
	{
		memmove(m_samples, m_samples + 1, sizeof(m_samples) - sizeof(T));
		m_samples[NumSamples - 1] = sample;
	}

	T Value()
	{
		T sum = 0;
		for (int i = 0; i < NumSamples; i++) {
			sum += m_samples[i];
		}
		return sum / NumSamples;
	}

	T Min()
	{
		T ret = m_samples[0];
		for (int i = 0; i < NumSamples; i++) {
			if (m_samples[i] < ret) {
				ret = m_samples[i];
			}
		}
		return ret;
	}

	T Max()
	{
		T ret = m_samples[0];
		for (int i = 0; i < NumSamples; i++) {
			if (m_samples[i] > ret) {
				ret = m_samples[i];
			}
		}
		return ret;
	}
};

NAMESPACE_END;

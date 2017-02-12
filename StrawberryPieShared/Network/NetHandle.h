#pragma once

#include <Common.h>

class NetHandle
{
public:
	uint32_t m_value = 0;

public:
	NetHandle()
	{
	}

	NetHandle(uint32_t value)
	{
		m_value = value;
	}

	bool IsNull()
	{
		return m_value == 0;
	}
};

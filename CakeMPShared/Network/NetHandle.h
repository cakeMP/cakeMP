#pragma once

#include <Common.h>

class NetHandle
{
public:
	uint32_t m_value = 0;

public:
	NetHandle() {}
	NetHandle(const NetHandle &copy) { m_value = copy.m_value; }
	NetHandle(uint32_t value) { m_value = value; }

	bool IsNull() { return m_value == 0; }

	operator uint32_t() const { return m_value; }
};

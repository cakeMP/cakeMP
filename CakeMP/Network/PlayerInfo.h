#pragma once

#include <Common.h>

#include <Network/NetHandle.h>

NAMESPACE_BEGIN;

class PlayerInfo
{
public:
	bool m_local = false;
	NetHandle m_handle;
	std::string m_username;
	std::string m_nickname;
};

NAMESPACE_END;

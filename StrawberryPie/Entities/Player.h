#pragma once

#include <Entities/Ped.h>

NAMESPACE_BEGIN;

class Player : public Ped
{
public:
	std::string m_username;
	std::string m_nickname;

public:
	Player();
	Player(int localHandle, const NetHandle &netHandle);
	virtual ~Player();
};

NAMESPACE_END;

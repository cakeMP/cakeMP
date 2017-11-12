#pragma once

#include <Entities/Ped.h>

#include <Network/Structs/CreatePed.h>

NAMESPACE_BEGIN;

class Player : public Ped
{
public:
	std::string m_username;
	std::string m_nickname;

public:
	Player();
	Player(const NetHandle &handle, const NetStructs::CreatePed &createPed);
	Player(int localHandle, const NetHandle &netHandle);
	virtual ~Player();

	virtual NetworkEntityType GetType();
};

NAMESPACE_END;

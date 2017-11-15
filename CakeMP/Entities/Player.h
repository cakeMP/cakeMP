#pragma once

#include <Entities/Ped.h>

#include <Network/Structs/CreatePed.h>
#include <Network/PlayerInfo.h>

NAMESPACE_BEGIN;

class Player : public Ped
{
protected:
	s2::ref<PlayerInfo> m_playerInfo;

public:
	Player();
	Player(const NetHandle &handle, const NetStructs::CreatePed &createPed);
	Player(int localHandle, const NetHandle &netHandle);
	virtual ~Player();

	virtual NetworkEntityType GetType();

	virtual s2::ref<PlayerInfo> GetPlayerInfo();
	virtual void SetPlayerInfo(const s2::ref<PlayerInfo> &playerInfo);
};

NAMESPACE_END;

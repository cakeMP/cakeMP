#pragma once

#include <Entities/Player.h>

NAMESPACE_BEGIN;

class Vehicle;

class LocalPlayer : public Player
{
public:
	int m_playerHandle = 0;

	glm::vec3 m_lastSyncedPosition;

	int m_enteringVehicleHandle = -1;
	int m_enteringVehicleSeat = -2;

	int m_inVehicleLocalHandle = -1;
	Vehicle* m_inVehicle = nullptr;
	int m_inVehicleSeat = -2;

public:
	LocalPlayer();
	virtual ~LocalPlayer();

	virtual bool CanBeDeleted();

	virtual int GetLocalHandle();
	virtual void SetLocalHandle(int handle);

	virtual void Delete();

	virtual void SetModel(uint32_t hash);

	void Initialize();

	virtual void Update();
	virtual void Frame();

	virtual s2::ref<PlayerInfo> GetPlayerInfo();
	virtual void SetPlayerInfo(const s2::ref<PlayerInfo> &playerInfo);
};

NAMESPACE_END;

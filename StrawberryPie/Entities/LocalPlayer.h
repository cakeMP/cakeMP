#pragma once

#include <Entities/Player.h>

NAMESPACE_BEGIN;

class LocalPlayer : public Player
{
public:
	int m_playerHandle = 0;

	glm::vec3 m_lastSyncedPosition;

	int m_enteringVehicleHandle = -1;

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
};

NAMESPACE_END;

#pragma once

#include <Entities/Ped.h>

NAMESPACE_BEGIN;

class LocalPlayer : public Ped
{
public:
	int m_playerHandle = 0;

	glm::vec3 m_lastSyncedPosition;

public:
	LocalPlayer();
	virtual ~LocalPlayer();

	virtual int GetHandle();
	virtual void SetHandle(int handle);

	virtual void SetModel(uint32_t hash);

	void Initialize();

	void Update();
};

NAMESPACE_END;

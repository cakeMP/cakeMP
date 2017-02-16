#pragma once

#include <Common.h>

#include <System/Entity.h>

NAMESPACE_BEGIN;

class Ped : public Entity
{
public:
	OnFootMoveTypes m_speedOnFoot;
	glm::vec3 m_speedOnFootTowards;

public:
	Ped();
	Ped(int localHandle, const NetHandle &netHandle);
	virtual ~Ped();

	virtual void Update();

	virtual void SetModel(uint32_t hash);

	static int CreateLocal(uint32_t modelHash, const glm::vec3 &pos, float heading = 0.0f);
};

NAMESPACE_END;

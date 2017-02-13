#pragma once

#include <Common.h>

#include <System/Entity.h>

NAMESPACE_BEGIN;

class Ped : public Entity
{
public:
	Ped();
	Ped(int localHandle, const NetHandle &netHandle);
	virtual ~Ped();

	virtual void SetModel(uint32_t hash);

	static int CreateLocal(uint32_t modelHash, const glm::vec3 &pos, float heading = 0.0f);
};

NAMESPACE_END;

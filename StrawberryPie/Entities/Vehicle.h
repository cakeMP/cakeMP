#pragma once

#include <Common.h>

#include <System/Entity.h>

#include <Network/Structs/CreateVehicle.h>

NAMESPACE_BEGIN;

class Vehicle : public Entity
{
public:
	Vehicle();
	Vehicle(int localHandle, const NetHandle &netHandle);
	Vehicle(const NetHandle &handle, const NetStructs::CreateVehicle &createVehicle);
	virtual ~Vehicle();

	virtual NetworkEntityType GetType();

	static int CreateLocal(uint32_t modelHash, const glm::vec3 &pos, float heading = 0.0f);
};

NAMESPACE_END;

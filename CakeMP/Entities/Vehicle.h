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

	virtual void SetPrimaryColor(int color);
	virtual int GetPrimaryColor();

	virtual void SetSecondaryColor(int color);
	virtual int GetSecondaryColor();

	virtual NetworkEntityType GetType();

	virtual void Update();

	static int CreateLocal(uint32_t modelHash, const glm::vec3 &pos, float heading = 0.0f);
};

NAMESPACE_END;

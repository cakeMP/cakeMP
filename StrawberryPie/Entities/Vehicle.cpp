#include <Common.h>
#include <Entities/Vehicle.h>

#include <Utils/Models.h>

#include <shv/natives.h>

NAMESPACE_BEGIN;

Vehicle::Vehicle()
{
}

Vehicle::Vehicle(int localHandle, const NetHandle &netHandle)
	: Entity(localHandle, netHandle)
{
}

Vehicle::Vehicle(const NetHandle &handle, const NetStructs::CreateVehicle &createVehicle)
{
	int localHandle = Vehicle::CreateLocal(createVehicle.m_model, createVehicle.m_position, createVehicle.m_rotation.z);

	SetLocalHandle(localHandle);
	SetNetHandle(handle);

	SetRotation(createVehicle.m_rotation);
}

Vehicle::~Vehicle()
{
}

void Vehicle::SetPrimaryColor(int color)
{
	VEHICLE::SET_VEHICLE_COLOURS(GetLocalHandle(), color, GetSecondaryColor());
}

int Vehicle::GetPrimaryColor()
{
	int primary, secondary;
	VEHICLE::GET_VEHICLE_COLOURS(GetLocalHandle(), &primary, &secondary);
	return primary;
}

void Vehicle::SetSecondaryColor(int color)
{
	VEHICLE::SET_VEHICLE_COLOURS(GetLocalHandle(), GetPrimaryColor(), color);
}

int Vehicle::GetSecondaryColor()
{
	int primary, secondary;
	VEHICLE::GET_VEHICLE_COLOURS(GetLocalHandle(), &primary, &secondary);
	return secondary;
}

NetworkEntityType Vehicle::GetType()
{
	return ET_Vehicle;
}

void Vehicle::Update()
{
}

int Vehicle::CreateLocal(uint32_t modelHash, const glm::vec3 &pos, float heading)
{
	if (!mdlRequest(modelHash)) {
		assert(false);
		modelHash = hashGet("voltic");
	}

	int ret = VEHICLE::CREATE_VEHICLE(modelHash, pos.x, pos.y, pos.z, heading, false, true);

	VEHICLE::SET_VEHICLE_COLOURS(ret, 0, 0);
	VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(ret, "QUINMP");

	return ret;
}

NAMESPACE_END;

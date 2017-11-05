#include <Common.h>

#include <Entities/Vehicle.h>

#include <Network/NetworkEntityType.h>
#include <Network/Structs/CreateVehicle.h>

Vehicle::Vehicle(const NetHandle &handle)
	: Entity(handle)
{
}

Vehicle::~Vehicle()
{
}

NetStructs::CreateVehicle Vehicle::GetNetworkCreateVehicleStruct()
{
	NetStructs::CreateVehicle ret;
	ret.m_model = m_model;
	ret.m_position = m_position;
	ret.m_rotation = m_rotation;
	return ret;
}

void Vehicle::NetworkSerialize(NetworkMessage* message)
{
	message->Write(ET_Vehicle);
	message->Write(GetNetworkCreateVehicleStruct());
}

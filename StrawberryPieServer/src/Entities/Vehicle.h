#pragma once

#include <Common.h>

#include <World/Entity.h>

#include <Network/NetHandle.h>
#include <Network/Structs/CreateVehicle.h>

class Vehicle : public Entity
{
public:
	uint32_t m_model = 0;

public:
	Vehicle(const NetHandle &handle);
	virtual ~Vehicle();

	NetStructs::CreateVehicle GetNetworkCreateVehicleStruct();

	virtual void NetworkSerialize(NetworkMessage* message);
};

#pragma once

#include <Common.h>

#include <World/Entity.h>

#include <Network/NetHandle.h>
#include <Network/Structs/CreateVehicle.h>

class Player;

struct VehicleSeat
{
	Player* m_player;
	int m_seat;

	VehicleSeat(Player* player, int seat)
	{
		m_player = player;
		m_seat = seat;
	}
};

class Vehicle : public Entity
{
private:
	uint32_t m_model = 0;

	std::vector<VehicleSeat> m_seats;

public:
	Vehicle(const NetHandle &handle);
	virtual ~Vehicle();

	virtual NetworkEntityType GetType();

	void SetModel(uint32_t model);
	inline uint32_t GetModel() { return m_model; }

	VehicleSeat* GetSeatWithPlayer(Player* player);
	VehicleSeat* GetSeatWithIndex(int seat);
	VehicleSeat* NewVehicleSeat(int seat);
	void FreeSeat(int seat);

	void AddOccupant(Player* player, int seat);
	void RemoveOccupant(Player* player);
	inline const std::vector<VehicleSeat> &GetSeats() { return m_seats; }

	NetStructs::CreateVehicle GetNetworkCreateVehicleStruct();
	virtual void NetworkSerialize(NetworkMessage* message);
};

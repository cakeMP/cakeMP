#include <Common.h>

#include <Entities/Vehicle.h>
#include <Entities/Player.h>

#include <Network/NetworkEntityType.h>
#include <Network/Structs/CreateVehicle.h>

Vehicle::Vehicle(const NetHandle &handle)
	: Entity(handle)
{
}

Vehicle::~Vehicle()
{
}

NetworkEntityType Vehicle::GetType()
{
	return ET_Vehicle;
}

void Vehicle::SetModel(uint32_t model)
{
	m_model = model;
	//TODO: Send message about updated vehicle model?
}

VehicleSeat* Vehicle::GetSeatWithPlayer(Player* player)
{
	for (VehicleSeat &s : m_seats) {
		if (s.m_player == player) {
			return &s;
		}
	}
	return nullptr;
}

VehicleSeat* Vehicle::GetSeatWithIndex(int seat)
{
	for (VehicleSeat &s : m_seats) {
		if (s.m_seat == seat) {
			return &s;
		}
	}
	return nullptr;
}

VehicleSeat* Vehicle::NewVehicleSeat(int seat)
{
	assert(GetSeatWithIndex(seat) == nullptr);

	m_seats.push_back(VehicleSeat(nullptr, seat));
	return &m_seats[m_seats.size()];
}

void Vehicle::FreeSeat(int seat)
{
	VehicleSeat* s = GetSeatWithIndex(seat);
	if (s == nullptr) {
		return;
	}
	s->m_player = nullptr;
}

void Vehicle::AddOccupant(Player* player, int seat)
{
	player->AddRef();

	assert(GetSeatWithPlayer(player) == nullptr);
	VehicleSeat* s = GetSeatWithIndex(seat);
	if (s != nullptr) {
		s = NewVehicleSeat(seat);
	}
	s->m_player = player;
}

void Vehicle::RemoveOccupant(Player* player)
{
	VehicleSeat* seat = GetSeatWithPlayer(player);
	if (seat != nullptr) {
		seat->m_player = nullptr;
	}
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
	message->Write(GetNetworkCreateVehicleStruct());
}

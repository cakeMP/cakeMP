#include <Common.h>

#include <Entities/LocalPlayer.h>
#include <Entities/Vehicle.h>

#include <System/Cake.h>
#include <Network/NetworkMessage.h>
#include <Utils/Models.h>
#include <Enums/GameControl.h>
#include <GTA/UI/UI.h>

#include <shv/natives.h>

NAMESPACE_BEGIN;

LocalPlayer::LocalPlayer()
{
}

LocalPlayer::~LocalPlayer()
{
}

bool LocalPlayer::CanBeDeleted()
{
	return false;
}

int LocalPlayer::GetLocalHandle()
{
	return PLAYER::GET_PLAYER_PED(Ped::GetLocalHandle());
}

void LocalPlayer::SetLocalHandle(int handle)
{
	m_playerHandle = handle;
}

void LocalPlayer::Delete()
{
}

void LocalPlayer::SetModel(uint32_t hash)
{
	if (!mdlRequest(hash)) {
		assert(false);
		return;
	}
	PLAYER::SET_PLAYER_MODEL(m_playerHandle, hash);
}

void LocalPlayer::Initialize()
{
	SetLocalHandle(PLAYER::GET_PLAYER_INDEX());

	m_username = PLAYER::GET_PLAYER_NAME(m_playerHandle);
	if (_pGame->m_settings.Nickname != "") {
		m_nickname = _pGame->m_settings.Nickname;
	} else {
		m_nickname = m_username;
	}
}

void LocalPlayer::Update()
{
	if (!_pGame->m_network.IsConnected()) {
		return;
	}

	glm::vec3 pos = GetPosition();
	glm::vec3 vel = GetVelocity();

	if (glm::length2(vel) > 0.5f) {
		if (m_inVehicle != nullptr) {
			logWrite("WARNING: VEHICLE MOVE SYNC IS NOT IMPLEMENTED YET!");
		} else {
			float heading = GetHeading();

			m_lastSyncedPosition = pos;

			uint8_t moveType = 0;
			if (AI::IS_PED_WALKING(GetLocalHandle())) {
				moveType = 1;
			} else if (AI::IS_PED_RUNNING(GetLocalHandle())) {
				moveType = 2;
			} else if (AI::IS_PED_SPRINTING(GetLocalHandle())) {
				moveType = 3;
			}

			NetworkMessage* msgPos = new NetworkMessage(NMT_PlayerMove);
			msgPos->Write(pos);
			msgPos->Write(heading);
			msgPos->Write(vel);
			msgPos->Write(moveType);
			_pGame->m_network.SendToHost(msgPos);
		}
	}

	if (PED::IS_PED_GETTING_INTO_A_VEHICLE(GetLocalHandle())) {
		int vehicleHandle = PED::GET_VEHICLE_PED_IS_TRYING_TO_ENTER(GetLocalHandle());

		if (vehicleHandle != 0 && m_enteringVehicleHandle != vehicleHandle) {
			m_enteringVehicleHandle = vehicleHandle;
			m_enteringVehicleSeat = PED::GET_SEAT_PED_IS_TRYING_TO_ENTER(GetLocalHandle());

			Vehicle* vehicle = _pGame->m_network.GetEntityFromLocalHandle<Vehicle>(ET_Vehicle, vehicleHandle);
			if (vehicle != nullptr) {
				logWrite("Entering vehicle %x @ %p", (uint32_t)vehicle->GetNetHandle(), vehicle);

				NetworkMessage* msgEnteringVehicle = new NetworkMessage(NMT_EnteringVehicle);
				msgEnteringVehicle->Write(vehicle->GetNetHandle());
				msgEnteringVehicle->Write(m_enteringVehicleSeat);
				_pGame->m_network.SendToHost(msgEnteringVehicle);
			}
		}
	} else {
		m_enteringVehicleHandle = -1;
	}

	if (PED::IS_PED_IN_ANY_VEHICLE(GetLocalHandle(), false)) {
		int vehicleLocalHandle = PED::GET_VEHICLE_PED_IS_IN(GetLocalHandle(), false);
		if (vehicleLocalHandle != m_inVehicleLocalHandle) {
			m_inVehicleLocalHandle = vehicleLocalHandle;

			Vehicle* vehicle = _pGame->m_network.GetEntityFromLocalHandle<Vehicle>(ET_Vehicle, vehicleLocalHandle);
			if (vehicle == nullptr) {
				logWrite("Player entered unsynced vehicle!");
			} else {
				m_inVehicle = vehicle;
				m_inVehicleSeat = m_enteringVehicleSeat;

				NetworkMessage* msgEnteredVehicle = new NetworkMessage(NMT_EnteredVehicle);
				msgEnteredVehicle->Write(vehicle->GetNetHandle());
				msgEnteredVehicle->Write(m_inVehicleSeat);
				_pGame->m_network.SendToHost(msgEnteredVehicle);
			}
		}
	} else {
		if (m_inVehicle != nullptr) {
			logWrite("Player has left vehicle!");

			NetworkMessage* msgEnteredVehicle = new NetworkMessage(NMT_LeftVehicle);
			msgEnteredVehicle->Write(m_inVehicle->GetNetHandle());
			msgEnteredVehicle->Write(m_inVehicleSeat);
			_pGame->m_network.SendToHost(msgEnteredVehicle);
		}

		m_inVehicleLocalHandle = -1;
		m_inVehicle = nullptr;
	}
}

void LocalPlayer::Frame()
{
	Player::Frame();

	if (!_pGame->m_network.IsConnected()) {
		return;
	}

	glm::vec3 pos = GetPosition();
	glm::vec3 vel = GetVelocity();
	GRAPHICS::DRAW_LINE(pos.x, pos.y, pos.z, pos.x + vel.x, pos.y + vel.y, pos.z + vel.z, 0, 0, 255, 255);
}

NAMESPACE_END;

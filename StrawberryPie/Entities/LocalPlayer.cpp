#include <Common.h>

#include <Entities/LocalPlayer.h>

#include <System/Strawberry.h>
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
	glm::vec3 pos = GetPosition();

	if (_pGame->m_network.IsConnected()) {
		glm::vec3 vel = GetVelocity();

		if (glm::length2(vel) > 0.5f) {
			glm::vec3 pos = GetPosition();
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

#include <Common.h>

#include <Entities/LocalPlayer.h>

#include <System/Strawberry.h>
#include <Network/NetworkMessage.h>

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
	m_nickname = m_username; //TODO
}

void LocalPlayer::Update()
{
	if (_pGame->m_network.m_connected) {
		glm::vec3 pos = GetPosition();

		if (glm::distance(m_lastSyncedPosition, pos) > 0.25f) {
			glm::vec3 rot = GetRotation();

			m_lastSyncedPosition = pos;

			NetworkMessage* msgPos = new NetworkMessage(NMT_PlayerMove);
			msgPos->Write(pos);
			msgPos->Write(rot);
			_pGame->m_network.SendToHost(msgPos);
		}
	}
}

NAMESPACE_END;

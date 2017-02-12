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

int LocalPlayer::GetHandle()
{
	return PLAYER::GET_PLAYER_PED(Ped::GetHandle());
}

void LocalPlayer::SetHandle(int handle)
{
	m_playerHandle = handle;
}

void LocalPlayer::SetModel(uint32_t hash)
{
	PLAYER::SET_PLAYER_MODEL(m_playerHandle, hash);
}

void LocalPlayer::Initialize()
{
	SetHandle(PLAYER::GET_PLAYER_INDEX());
}

void LocalPlayer::Update()
{
	glm::vec3 pos = GetPosition();
	glm::vec3 rot = GetRotation();

	if (glm::distance(m_lastSyncedPosition, pos) > 0.25f) {
		NetworkMessage* msgPos = new NetworkMessage(NMT_PlayerMove);
		msgPos->Write(pos);
		msgPos->Write(rot);
		_pGame->m_network.SendToHost(msgPos);
	}
}

NAMESPACE_END;

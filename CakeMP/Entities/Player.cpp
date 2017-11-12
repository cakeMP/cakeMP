#include <Common.h>

#include <Entities/Player.h>

NAMESPACE_BEGIN;

Player::Player()
{
}

Player::Player(const NetHandle &handle, const NetStructs::CreatePed &createPed)
{
	int localHandle = Ped::CreateLocal(createPed.m_model, createPed.m_position, createPed.m_rotation.z);

	SetLocalHandle(localHandle);
	SetNetHandle(handle);

	SetRotation(createPed.m_rotation);
}

Player::Player(int localHandle, const NetHandle &netHandle)
	: Ped(localHandle, netHandle)
{
}

Player::~Player()
{
}

NetworkEntityType Player::GetType()
{
	return ET_Player;
}

NAMESPACE_END;

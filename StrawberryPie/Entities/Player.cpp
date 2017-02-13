#include <Common.h>

#include <Entities/Player.h>

NAMESPACE_BEGIN;

Player::Player()
{
}

Player::Player(const NetStructs::CreatePed &createPed)
{
	int localHandle = Ped::CreateLocal(createPed.m_model, createPed.m_position, createPed.m_rotation.z);

	SetLocalHandle(localHandle);
	SetNetHandle(createPed.m_handle);

	SetRotation(createPed.m_rotation);
}

Player::Player(int localHandle, const NetHandle &netHandle)
	: Ped(localHandle, netHandle)
{
}

Player::~Player()
{
}

NAMESPACE_END;

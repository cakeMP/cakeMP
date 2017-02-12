#include <Common.h>

#include <Entities/Player.h>

NAMESPACE_BEGIN;

Player::Player()
{
}

Player::Player(int localHandle, const NetHandle &netHandle)
	: Ped(localHandle, netHandle)
{
}

Player::~Player()
{
}

NAMESPACE_END;

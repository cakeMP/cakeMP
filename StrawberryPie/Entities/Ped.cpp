#include <Common.h>

#include <Entities/Ped.h>

#include <shv/natives.h>

NAMESPACE_BEGIN;

Ped::Ped()
{
}

Ped::Ped(int localHandle, const NetHandle &netHandle)
	: Entity(localHandle, netHandle)
{
}

Ped::~Ped()
{
}

void Ped::SetModel(uint32_t hash)
{
	//TODO: We have to re-create the entire ped, I think?
}

NAMESPACE_END;

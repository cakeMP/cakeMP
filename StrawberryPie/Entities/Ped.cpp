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

int Ped::CreateLocal(uint32_t modelHash, const glm::vec3 &pos, float heading)
{
	if (!mdlRequest(modelHash)) {
		assert(false);
		modelHash = hashGet("player_one");
	}
	return PED::CREATE_PED(26, modelHash, pos.x, pos.y, pos.z, heading, false, true);
}

NAMESPACE_END;

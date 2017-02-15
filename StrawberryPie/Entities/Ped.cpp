#include <Common.h>

#include <Entities/Ped.h>

#include <System/Strawberry.h>

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

	int ret = PED::CREATE_PED(26, modelHash, pos.x, pos.y, pos.z, heading, false, true);

	ENTITY::SET_ENTITY_INVINCIBLE(ret, true);
	ENTITY::FREEZE_ENTITY_POSITION(ret, true);

	PED::SET_PED_CAN_BE_TARGETTED(ret, true);
	PED::SET_PED_CAN_BE_TARGETTED_BY_PLAYER(ret, _pGame->m_player.GetLocalHandle(), true);
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ret, true);
	AI::TASK_SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ret, true);
	PED::SET_PED_RELATIONSHIP_GROUP_HASH(ret, _pGame->m_pedRelGroup);
	PED::SET_PED_CAN_RAGDOLL(ret, false);
	PED::SET_PED_DEFAULT_COMPONENT_VARIATION(ret);
	PED::SET_PED_CAN_EVASIVE_DIVE(ret, false);
	PED::SET_PED_GET_OUT_UPSIDE_DOWN_VEHICLE(ret, false);
	PED::SET_PED_AS_ENEMY(ret, false);
	PED::SET_CAN_ATTACK_FRIENDLY(ret, true, false);

	return ret;
}

NAMESPACE_END;

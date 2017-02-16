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

void Ped::Update()
{
	Entity::Update();

	if (m_lerpPos.IsActive()) {
		glm::vec3 &from = m_lerpPos.From();
		glm::vec3 &to = m_lerpPos.To();

		GRAPHICS::DRAW_LINE(from.x, from.y, from.z, to.x, to.y, to.z, 255, 0, 0, 255);
		GRAPHICS::DRAW_LINE(from.x, from.y, from.z, m_speedOnFootTowards.x, m_speedOnFootTowards.y, m_speedOnFootTowards.z, 0, 255, 0, 255);
	} else {
		m_speedOnFoot = OFMT_Still;
	}

	if (m_speedOnFoot == OFMT_Still) {
		AI::CLEAR_PED_TASKS(GetLocalHandle());

	} else if (m_speedOnFoot == OFMT_Walking) {
		AI::TASK_GO_STRAIGHT_TO_COORD(GetLocalHandle(), m_speedOnFootTowards.x, m_speedOnFootTowards.y, m_speedOnFootTowards.z, 1.0f, -1, 0.0f, 0.0f);
		AI::SET_PED_DESIRED_MOVE_BLEND_RATIO(GetLocalHandle(), 1.0f);

	} else if (m_speedOnFoot == OFMT_Running) {
		AI::TASK_GO_STRAIGHT_TO_COORD(GetLocalHandle(), m_speedOnFootTowards.x, m_speedOnFootTowards.y, m_speedOnFootTowards.z, 4.0f, -1, 0.0f, 0.0f);
		AI::SET_PED_DESIRED_MOVE_BLEND_RATIO(GetLocalHandle(), 2.0f);

	} else if (m_speedOnFoot == OFMT_Sprinting) {
		AI::TASK_GO_STRAIGHT_TO_COORD(GetLocalHandle(), m_speedOnFootTowards.x, m_speedOnFootTowards.y, m_speedOnFootTowards.z, 3.0f, -1, 0.0f, 0.0f);
		PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(GetLocalHandle(), 1.49f);
		AI::SET_PED_DESIRED_MOVE_BLEND_RATIO(GetLocalHandle(), 3.0f);
	}
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

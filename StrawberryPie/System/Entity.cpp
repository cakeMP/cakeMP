#include <Common.h>

#include <System/Entity.h>

#include <System/Strawberry.h>

#include <shv/natives.h>

NAMESPACE_BEGIN;

Entity::Entity()
{
}

Entity::Entity(int localHandle, const NetHandle &netHandle)
{
	m_handle = localHandle;
	m_netHandle = netHandle;
}

Entity::~Entity()
{
	Delete();
}

void Entity::Update()
{
	if (m_lerpPosStart > 0) {
		int tmNow = _pGame->m_gameTime;
		int tmPassed = tmNow - m_lerpPosStart;
		float factor = tmPassed / (float)m_lerpPosLength;

		if (factor >= 1.0f) {
			m_lerpPosStart = 0;
			factor = 1.0f;
		}

		glm::vec3 lerped = glm::mix(m_lerpPosFrom, m_lerpPosTo, factor);
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(GetLocalHandle(), lerped.x, lerped.y, lerped.z, false, false, false);

		GRAPHICS::DRAW_LINE(m_lerpPosFrom.x, m_lerpPosFrom.y, m_lerpPosFrom.z, m_lerpPosTo.x, m_lerpPosTo.y, m_lerpPosTo.z, 255, 0, 0, 255);
		GRAPHICS::DRAW_LINE(m_lerpPosFrom.x, m_lerpPosFrom.y, m_lerpPosFrom.z, m_TEMP_predictPos.x, m_TEMP_predictPos.y, m_TEMP_predictPos.z, 0, 255, 0, 255);
	}
}

void Entity::Interpolate(const glm::vec3 &start, const glm::vec3 &end, int ms)
{
	m_lerpPosStart = _pGame->m_gameTime;
	m_lerpPosLength = ms;
	m_lerpPosFrom = start;
	m_lerpPosTo = end;

	ENTITY::SET_ENTITY_COORDS_NO_OFFSET(GetLocalHandle(), start.x, start.y, start.z, false, false, false);
}

bool Entity::IsLocal()
{
	return m_netHandle.IsNull();
}

bool Entity::CanBeDeleted()
{
	return IsLocal();
}

int Entity::GetLocalHandle()
{
	return m_handle;
}

void Entity::SetLocalHandle(int handle)
{
	m_handle = handle;
}

NetHandle Entity::GetNetHandle()
{
	return m_netHandle;
}

void Entity::SetNetHandle(const NetHandle &handle)
{
	m_netHandle = handle;
}

void Entity::Delete()
{
	int handle = GetLocalHandle();
	ENTITY::SET_ENTITY_AS_MISSION_ENTITY(handle, false, true);
	ENTITY::DELETE_ENTITY(&handle);
	SetLocalHandle(handle);
}

bool Entity::IsDead()
{
	return (ENTITY::IS_ENTITY_DEAD(GetLocalHandle()) == 1);
}

glm::vec3 Entity::GetPosition()
{
	Vector3 ret = ENTITY::GET_ENTITY_COORDS(GetLocalHandle(), !IsDead());
	return glm::vec3(ret.x, ret.y, ret.z);
}

void Entity::SetPosition(const glm::vec3 &pos)
{
	ENTITY::SET_ENTITY_COORDS(GetLocalHandle(), pos.x, pos.y, pos.z, false, false, false, true);
}

void Entity::SetPositionNoOffset(const glm::vec3 &pos)
{
	ENTITY::SET_ENTITY_COORDS_NO_OFFSET(GetLocalHandle(), pos.x, pos.y, pos.z, false, false, false);
}

glm::vec3 Entity::GetRotation()
{
	Vector3 ret = ENTITY::GET_ENTITY_ROTATION(GetLocalHandle(), 2);
	return glm::vec3(ret.x, ret.y, ret.z);
}

void Entity::SetRotation(const glm::vec3 &rot)
{
	ENTITY::SET_ENTITY_ROTATION(GetLocalHandle(), rot.x, rot.y, rot.z, 2, true);
}

glm::vec3 Entity::GetVelocity()
{
	Vector3 ret = ENTITY::GET_ENTITY_VELOCITY(GetLocalHandle());
	return glm::vec3(ret.x, ret.y, ret.z);
}

void Entity::SetVelocity(const glm::vec3 &vel)
{
	ENTITY::SET_ENTITY_VELOCITY(GetLocalHandle(), vel.x, vel.y, vel.z);
}

uint32_t Entity::GetModel()
{
	return ENTITY::GET_ENTITY_MODEL(GetLocalHandle());
}

NAMESPACE_END;

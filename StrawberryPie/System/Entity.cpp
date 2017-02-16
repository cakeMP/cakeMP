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
	if (m_lerpPos.IsActive()) {
		m_lerpPos.Update();
		glm::vec3 lerpedPosition = m_lerpPos.Current();
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(GetLocalHandle(), lerpedPosition.x, lerpedPosition.y, lerpedPosition.z, false, false, false);
	}

	if (m_lerpRot.IsActive()) {
		m_lerpRot.Update();
		glm::vec3 lerpedRotation = m_lerpRot.Current();
		ENTITY::SET_ENTITY_ROTATION(GetLocalHandle(), lerpedRotation.x, lerpedRotation.y, lerpedRotation.z, 2, true);
	}

	if (m_lerpHeading.IsActive()) {
		m_lerpHeading.Update();
		ENTITY::SET_ENTITY_HEADING(GetLocalHandle(), m_lerpHeading.Current());
	}
}

void Entity::InterpolatePosition(const glm::vec3 &start, const glm::vec3 &end, int ms)
{
	m_lerpPos.Set(start, end, ms);
	ENTITY::SET_ENTITY_COORDS_NO_OFFSET(GetLocalHandle(), start.x, start.y, start.z, false, false, false);
}

void Entity::InterpolateRotation(const glm::vec3 &start, const glm::vec3 &end, int ms)
{
	m_lerpRot.Set(start, end, ms);
	ENTITY::SET_ENTITY_ROTATION(GetLocalHandle(), start.x, start.y, start.z, 2, true);
}

void Entity::InterpolateHeading(float start, float end, int ms)
{
	m_lerpHeading.Set(start, end, ms);
	ENTITY::SET_ENTITY_HEADING(GetLocalHandle(), start);
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

float Entity::GetHeading()
{
	return ENTITY::GET_ENTITY_HEADING(GetLocalHandle());
}

void Entity::SetHeading(float heading)
{
	ENTITY::SET_ENTITY_HEADING(GetLocalHandle(), heading);
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

#include <Common.h>

#include <System/Entity.h>

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

uint32_t Entity::GetModel()
{
	return ENTITY::GET_ENTITY_MODEL(GetLocalHandle());
}

NAMESPACE_END;

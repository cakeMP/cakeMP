#include <Common.h>

#include <System/Entity.h>

#include <shv/natives.h>

NAMESPACE_BEGIN;

Entity::Entity()
{
}

Entity::~Entity()
{
}

int Entity::GetHandle()
{
	return m_handle;
}

void Entity::SetHandle(int handle)
{
	m_handle = handle;
}

bool Entity::IsDead()
{
	return (ENTITY::IS_ENTITY_DEAD(GetHandle()) == 1);
}

glm::vec3 Entity::GetPosition()
{
	Vector3 ret = ENTITY::GET_ENTITY_COORDS(GetHandle(), !IsDead());
	return glm::vec3(ret.x, ret.y, ret.z);
}

void Entity::SetPosition(const glm::vec3 &pos)
{
	ENTITY::SET_ENTITY_COORDS(GetHandle(), pos.x, pos.y, pos.z, false, false, false, true);
}

glm::vec3 Entity::GetRotation()
{
	Vector3 ret = ENTITY::GET_ENTITY_ROTATION(GetHandle(), 2);
	return glm::vec3(ret.x, ret.y, ret.z);
}

void Entity::SetRotation(const glm::vec3 &rot)
{
	ENTITY::SET_ENTITY_ROTATION(GetHandle(), rot.x, rot.y, rot.z, 2, true);
}

uint32_t Entity::GetModel()
{
	return ENTITY::GET_ENTITY_MODEL(GetHandle());
}

NAMESPACE_END;

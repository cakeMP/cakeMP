#include <Common.h>

#include <World/Entity.h>
#include <GameServer.h>

Entity::Entity(const NetHandle &handle)
{
	m_handle = handle;
}

Entity::~Entity()
{
}

void Entity::Update()
{
}

void Entity::SetPosition(const glm::vec3 &pos)
{
	glm::vec3 oldPos = m_position;
	m_position = pos;
	_pServer->m_world.EntityMoved(this, oldPos);
}

void Entity::SetRotation(const glm::vec3 &rot)
{
	glm::vec3 oldRot = m_rotation;
	m_rotation = rot;
	_pServer->m_world.EntityMoved(this, oldRot);
}

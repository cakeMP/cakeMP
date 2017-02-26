#include <Common.h>

#include <World/Entity.h>

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

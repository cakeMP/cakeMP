#include <Common.h>

#include <Entity.h>

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

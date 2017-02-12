#pragma once

#include <Common.h>
#include <Network/NetHandle.h>

class Entity
{
public:
	NetHandle m_handle;

	glm::vec3 m_position;
	glm::vec3 m_rotation;

public:
	Entity(const NetHandle &handle);
	virtual ~Entity();

	virtual void Update();
};

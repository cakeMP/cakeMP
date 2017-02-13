#pragma once

#include <Common.h>
#include <Network/NetHandle.h>
#include <Network/NetworkMessage.h>

class Entity
{
public:
	NetHandle m_handle;

	glm::vec3 m_position;
	glm::vec3 m_rotation;

	uint32_t m_model = 0;

public:
	Entity(const NetHandle &handle);
	virtual ~Entity();

	//TODO: Make this can be done cleaner..
	virtual void NetworkSerialize(NetworkMessage* message) = 0;

	virtual void Update();
};

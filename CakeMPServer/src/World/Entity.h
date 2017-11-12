#pragma once

#include <Common.h>
#include <Network/NetHandle.h>
#include <Network/NetworkMessage.h>
#include <Network/NetworkEntityType.h>

class Entity : public RefCounted
{
public:
	NetHandle m_handle;

protected:
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_velocity;
	uint8_t m_moveType = 0;

public:
	Entity(const NetHandle &handle);
	virtual ~Entity();

	virtual NetworkEntityType GetType() = 0;

	//TODO: Make this can be done cleaner..
	virtual void NetworkSerialize(NetworkMessage* message) = 0;

	virtual void Update();

	void SetPosition(const glm::vec3 &pos);
	inline glm::vec3 GetPosition() { return m_position; }

	void SetRotation(const glm::vec3 &rot);
	inline glm::vec3 GetRotation() { return m_rotation; }
};

#pragma once

#include <Common.h>

NAMESPACE_BEGIN;

class Entity
{
private:
	int m_handle = 0;

public:
	Entity();
	virtual ~Entity();

	virtual int GetHandle();
	virtual void SetHandle(int handle);

	virtual bool IsDead();

	virtual glm::vec3 GetPosition();
	virtual void SetPosition(const glm::vec3 &pos);

	virtual glm::vec3 GetRotation();
	virtual void SetRotation(const glm::vec3 &rot);

	virtual uint32_t GetModel();
};

NAMESPACE_END;

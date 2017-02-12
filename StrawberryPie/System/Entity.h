#pragma once

#include <Common.h>

#include <Network/NetHandle.h>

NAMESPACE_BEGIN;

class Entity
{
private:
	int m_handle = 0;
	NetHandle m_netHandle;

public:
	Entity();
	Entity(int localHandle, const NetHandle &netHandle);
	virtual ~Entity();

	virtual bool IsLocal();
	virtual bool CanBeDeleted();

	virtual int GetLocalHandle();
	virtual void SetLocalHandle(int handle);

	virtual NetHandle GetNetHandle();
	virtual void SetNetHandle(const NetHandle &handle);

	virtual void Delete();

	virtual bool IsDead();

	virtual glm::vec3 GetPosition();
	virtual void SetPosition(const glm::vec3 &pos);

	virtual glm::vec3 GetRotation();
	virtual void SetRotation(const glm::vec3 &rot);

	virtual uint32_t GetModel();
};

NAMESPACE_END;

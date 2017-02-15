#pragma once

#include <Common.h>

#include <Network/NetHandle.h>

NAMESPACE_BEGIN;

class Entity
{
private:
	int m_handle = 0;
	NetHandle m_netHandle;

	int m_lerpPosStart = 0;
	int m_lerpPosLength = 0;
	glm::vec3 m_lerpPosFrom;
	glm::vec3 m_lerpPosTo;

public:
	glm::vec3 m_TEMP_predictPos; //TODO: Remove this

public:
	Entity();
	Entity(int localHandle, const NetHandle &netHandle);
	virtual ~Entity();

	virtual void Update();

	virtual void Interpolate(const glm::vec3 &start, const glm::vec3 &end, int ms);

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
	virtual void SetPositionNoOffset(const glm::vec3 &pos);

	virtual glm::vec3 GetRotation();
	virtual void SetRotation(const glm::vec3 &rot);

	virtual glm::vec3 GetVelocity();
	virtual void SetVelocity(const glm::vec3 &vel);

	virtual uint32_t GetModel();
};

NAMESPACE_END;

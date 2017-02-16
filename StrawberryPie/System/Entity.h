#pragma once

#include <Common.h>

#include <Network/NetHandle.h>
#include <System/Interpolator.h>

NAMESPACE_BEGIN;

class Entity
{
private:
	int m_handle = 0;
	NetHandle m_netHandle;

protected:
	Interpolator<glm::vec3> m_lerpPos;
	Interpolator<glm::quat> m_lerpRot;

public:
	Entity();
	Entity(int localHandle, const NetHandle &netHandle);
	virtual ~Entity();

	virtual void Update();

	virtual void InterpolatePosition(const glm::vec3 &start, const glm::vec3 &end, int ms);
	virtual void InterpolateQuaternion(const glm::quat &start, const glm::quat &end, int ms);
	virtual void InterpolateHeading(float start, float end, int ms);

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

	virtual glm::quat GetQuat();
	virtual void SetQuat(const glm::quat &quat);

	virtual float GetHeading();
	virtual void SetHeading(float heading);

	virtual glm::vec3 GetVelocity();
	virtual void SetVelocity(const glm::vec3 &vel);

	virtual uint32_t GetModel();
};

NAMESPACE_END;

#pragma once

#include <Common.h>
#include <System/Entity.h>

NAMESPACE_BEGIN;

class ScriptCam
{
public:
	int m_handle;

public:
	ScriptCam();
	virtual ~ScriptCam();

	virtual void SetActive(bool active = true);
	virtual bool IsActive();
	virtual bool IsRendering();

	virtual void SetPosition(const glm::vec3 &pos);
	virtual glm::vec3 GetPosition();

	virtual void SetRotation(const glm::vec3 &rot);
	virtual glm::vec3 GetRotation();

	virtual void SetFOV(float fov);
	virtual float GetFOV();

	virtual void PointAt(const glm::vec3 &pos);
	virtual void PointAt(Entity* ent);
	virtual void PointAt(int localHandle);
};

void camFadeIn(int duration);
void camFadeOut(int duration);

void camScriptCams(bool active);

NAMESPACE_END;

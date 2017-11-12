#include <Common.h>
#include <GTA/ScriptCam.h>

#include <shv/natives.h>

NAMESPACE_BEGIN;

ScriptCam::ScriptCam()
{
	m_handle = CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", false);
}

ScriptCam::~ScriptCam()
{
	CAM::DESTROY_CAM(m_handle, false);
}

void ScriptCam::SetActive(bool active)
{
	CAM::SET_CAM_ACTIVE(m_handle, active);
}

bool ScriptCam::IsActive()
{
	return (CAM::IS_CAM_ACTIVE(m_handle) == TRUE);
}

bool ScriptCam::IsRendering()
{
	return (CAM::IS_CAM_RENDERING(m_handle) == TRUE);
}

void ScriptCam::SetPosition(const glm::vec3 &pos)
{
	CAM::SET_CAM_COORD(m_handle, pos.x, pos.y, pos.z);
}

glm::vec3 ScriptCam::GetPosition()
{
	Vector3 v3 = CAM::GET_CAM_COORD(m_handle);
	return glm::vec3(v3.x, v3.y, v3.z);
}

void ScriptCam::SetRotation(const glm::vec3 &rot)
{
	CAM::SET_CAM_ROT(m_handle, rot.x, rot.y, rot.z, 2);
}

glm::vec3 ScriptCam::GetRotation()
{
	Vector3 v3 = CAM::GET_CAM_ROT(m_handle, 2);
	return glm::vec3(v3.x, v3.y, v3.z);
}

void ScriptCam::SetFOV(float fov)
{
	CAM::SET_CAM_FOV(m_handle, fov);
}

float ScriptCam::GetFOV()
{
	return CAM::GET_CAM_FOV(m_handle);
}

void ScriptCam::PointAt(const glm::vec3 &pos)
{
	CAM::POINT_CAM_AT_COORD(m_handle, pos.x, pos.y, pos.z);
}

void ScriptCam::PointAt(Entity* ent)
{
	if (ent == nullptr) {
		PointAt(-1);
	} else {
		PointAt(ent->GetLocalHandle());
	}
}

void ScriptCam::PointAt(int localHandle)
{
	if (localHandle == -1) {
		CAM::STOP_CAM_POINTING(m_handle);
	} else {
		CAM::POINT_CAM_AT_ENTITY(m_handle, localHandle, 0, 0, 0, true);
	}
}

void camFadeIn(int duration)
{
	CAM::DO_SCREEN_FADE_IN(duration);
}

void camFadeOut(int duration)
{
	CAM::DO_SCREEN_FADE_OUT(duration);
}

void camScriptCams(bool active)
{
	CAM::RENDER_SCRIPT_CAMS(active, false, 0, false, false);
}

NAMESPACE_END;

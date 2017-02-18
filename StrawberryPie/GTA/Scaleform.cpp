#include <Common.h>

#include <GTA/Scaleform.h>

#include <shv/natives.h>

NAMESPACE_BEGIN;

Scaleform::Scaleform()
{
}

Scaleform::Scaleform(const char* id)
{
	Load(id);
}

Scaleform::~Scaleform()
{
	GRAPHICS::SET_SCALEFORM_MOVIE_AS_NO_LONGER_NEEDED(&m_handle);
}

void Scaleform::Load(const char* id)
{
	m_handle = GRAPHICS::REQUEST_SCALEFORM_MOVIE(id);
}

bool Scaleform::IsValid()
{
	return m_handle != 0;
}

bool Scaleform::IsLoaded()
{
	return (GRAPHICS::HAS_SCALEFORM_MOVIE_LOADED(m_handle) == TRUE);
}

void Scaleform::StartFunction(const char* name)
{
	if (!IsValid()) {
		return;
	}
	GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(m_handle, name);
}

void Scaleform::PushParam(int v)
{
	if (!IsValid()) {
		return;
	}
	GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(v);
}

void Scaleform::PushParam(float v)
{
	if (!IsValid()) {
		return;
	}
	GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_FLOAT(v);
}

void Scaleform::PushParam(bool v)
{
	if (!IsValid()) {
		return;
	}
	GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_BOOL(v);
}

void Scaleform::PushParam(const char* v)
{
	if (!IsValid()) {
		return;
	}
	GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING(v);
}

void Scaleform::PushParam(const std::string &v)
{
	if (!IsValid()) {
		return;
	}
	GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING(v.c_str());
}

void Scaleform::FinishFunction()
{
	if (!IsValid()) {
		return;
	}
	GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();
}

void Scaleform::CallFunction(const char* name)
{
	if (!IsValid()) {
		return;
	}
	GRAPHICS::CALL_SCALEFORM_MOVIE_METHOD(m_handle, name);
}

void Scaleform::Render()
{
	if (!IsValid()) {
		return;
	}
	GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN(m_handle, 255, 255, 255, 255, 0);
}

void Scaleform::Render(const glm::vec2 &pos, const glm::vec2 &size)
{
	if (!IsValid()) {
		return;
	}
	GRAPHICS::DRAW_SCALEFORM_MOVIE(m_handle, pos.x, pos.y, size.x, size.y, 255, 255, 255, 255, 0);
}

void Scaleform::Render3D(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &scale, bool additive)
{
	if (!IsValid()) {
		return;
	}
	if (additive) {
		GRAPHICS::DRAW_SCALEFORM_MOVIE_3D(m_handle,
			pos.x, pos.y, pos.z,
			rot.x, rot.y, rot.z,
			2.0f, 2.0f, 1.0f,
			scale.x, scale.y, scale.z,
			2);
	} else {
		GRAPHICS::_DRAW_SCALEFORM_MOVIE_3D_NON_ADDITIVE(m_handle,
			pos.x, pos.y, pos.z,
			rot.x, rot.y, rot.z,
			2.0f, 2.0f, 1.0f,
			scale.x, scale.y, scale.z,
			2);
	}
}

NAMESPACE_END;

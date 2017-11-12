#pragma once

#include <Common.h>

NAMESPACE_BEGIN;

class Scaleform
{
private:
	int m_handle = 0;

public:
	Scaleform();
	Scaleform(const char* id);
	~Scaleform();

	void Load(const char* id);
	bool IsValid();
	bool IsLoaded();

	void StartFunction(const char* name);
	void PushParam(int v);
	void PushParam(float v);
	void PushParam(bool v);
	void PushParam(const char* v);
	void PushParam(const std::string &v);
	void FinishFunction();

	template<typename T>
	T FinishFunctionReturn()
	{
		if (!IsValid()) {
			return (T)0;
		}
		return (T)GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION();
	}

	void CallFunction(const char* name);
	template<typename T>
	T ReturnFunction(const char* name)
	{
		if (!IsValid()) {
			return (T)0;
		}
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(m_handle, name);
		return (T)GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION();
	}

	void Render();
	void Render(const glm::vec2 &pos, const glm::vec2 &size);

	void Render3D(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &scale, bool additive = false);
};

NAMESPACE_END;

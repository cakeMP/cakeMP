#pragma once

#include <Common.h>

NAMESPACE_BEGIN;

class UITexture
{
private:
	std::string m_dict;
	std::string m_name;

public:
	UITexture();
	UITexture(const std::string &dict, const std::string &name);

	const char* Dict() const;
	const char* Name() const;
	glm::ivec2 Size() const;

	void Set(const std::string &dict, const std::string &name);

	void EnsureLoaded() const;

	void Render(const glm::vec2 &pos);
	void Render(const glm::vec2 &pos, const glm::vec2 &size) const;
	void Render(const glm::vec2 &pos, const glm::vec2 &size, float heading) const;
	void Render(const glm::vec2 &pos, const glm::vec2 &size, float heading, const glm::vec4 &color) const;
	void Render(const glm::vec2 &pos, const glm::vec2 &size, float heading, const glm::ivec4 &color) const;
};

NAMESPACE_END;

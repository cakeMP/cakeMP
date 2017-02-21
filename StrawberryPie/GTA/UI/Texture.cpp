#include <Common.h>

#include <GTA/UI/Texture.h>

#include <GTA/UI/UI.h>

#include <shv/natives.h>

NAMESPACE_BEGIN;

UITexture::UITexture()
{
}

UITexture::UITexture(const std::string &dict, const std::string &name)
{
	Set(dict, name);
}

const char* UITexture::Dict() const
{
	return m_dict.c_str();
}

const char* UITexture::Name() const
{
	return m_name.c_str();
}

glm::ivec2 UITexture::Size() const
{
	Vector3 v = GRAPHICS::GET_TEXTURE_RESOLUTION(m_dict.c_str(), m_name.c_str());
	return glm::ivec2((int)v.x, (int)v.y);
}

void UITexture::Set(const std::string &dict, const std::string &name)
{
	m_dict = dict;
	m_name = name;

	EnsureLoaded();
}

void UITexture::EnsureLoaded() const
{
	if (!GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED(m_dict.c_str())) {
		GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT(m_dict.c_str(), true);
	}
}

void UITexture::Render(const glm::vec2 &pos)
{
	Render(pos, Size(), 0.0f, glm::ivec4(255, 255, 255, 255));
}

void UITexture::Render(const glm::vec2 &pos, const glm::vec2 &size) const
{
	Render(pos, size, 0.0f, glm::ivec4(255, 255, 255, 255));
}

void UITexture::Render(const glm::vec2 &pos, const glm::vec2 &size, float heading) const
{
	Render(pos, size, heading, glm::ivec4(255, 255, 255, 255));
}

void UITexture::Render(const glm::vec2 &pos, const glm::vec2 &size, float heading, const glm::vec4 &color) const
{
	Render(pos, size, heading, uiColor(color));
}

void UITexture::Render(const glm::vec2 &pos, const glm::vec2 &size, float heading, const glm::ivec4 &color) const
{
	EnsureLoaded();

	glm::vec2 spos = uiScreenScale(uiRectCenter(pos, size));
	glm::vec2 ssize = uiScreenScale(size);

	GRAPHICS::DRAW_SPRITE(m_dict.c_str(), m_name.c_str(), spos.x, spos.y, ssize.x, ssize.y, heading, color.r, color.g, color.b, color.a);
}

NAMESPACE_END;

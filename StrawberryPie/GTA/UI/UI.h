#pragma once

#include <Common.h>

#include <GTA/UI/Texture.h>

NAMESPACE_BEGIN;

extern int ui_screenWidth;
extern int ui_screenHeight;

extern void uiAddLongString(const char* str);

extern void uiNotify(const char* str);
extern void uiNotify(const std::string &str);

extern void uiDrawRectangle(const glm::vec2 &pos, const glm::vec2 &size, const glm::vec4 &color);

// Transforms a normalized color [0..1] into an integer color [0..255]
inline glm::ivec4 uiColor(const glm::vec4 &col)
{
	return glm::ivec4(
		(int)(col.r * 255.0f),
		(int)(col.g * 255.0f),
		(int)(col.b * 255.0f),
		(int)(col.a * 255.0f));
}

// Transforms an integer color [0..255] into a normalized color [0..1]
inline glm::vec4 uiColor(const glm::ivec4 &col)
{
	return glm::vec4(
		col.r / 255.0f,
		col.g / 255.0f,
		col.b / 255.0f,
		col.a / 255.0f);
}

// Get the center of the given rectangle
inline glm::vec2 uiRectCenter(const glm::vec2 &pos, const glm::vec2 &size)
{
	return glm::vec2(pos.x + size.x / 2.0f, pos.y + size.y / 2.0f);
}

// Scale a vector by the screen width and height
inline glm::vec2 uiScreenScale(const glm::vec2 &v)
{
	return glm::vec2(v.x / (float)ui_screenWidth, v.y / (float)ui_screenHeight);
}

NAMESPACE_END;

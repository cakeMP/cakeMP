#pragma once

#include <Common.h>

NAMESPACE_BEGIN;

enum UITextAlignment
{
	UITA_Left,
	UITA_Center,
	UITA_Right
};

class UIText
{
public:
	std::string m_text;

	int m_font = 4;
	glm::vec4 m_color;
	float m_scale = 1.0f;
	UITextAlignment m_align = UITA_Left;

	bool m_dropShadow = false;
	bool m_outline = false;

	bool m_wrapping = false;
	glm::vec2 m_wrapSize;

public:
	UIText();
	UIText(const std::string &text);
	~UIText();

	void Render(const glm::vec2 &pos);

public:
	static float Measure(const std::string &text, int font, float scale = 1.0f);
};

NAMESPACE_END;

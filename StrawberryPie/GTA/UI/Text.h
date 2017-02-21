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
	float m_wrapSize = 0.0f;

public:
	UIText();
	UIText(const std::string &text);
	~UIText();

	float Measure();
	int LineCount();

	void Render(const glm::vec2 &pos);
};

NAMESPACE_END;

#pragma once

#include <Common.h>

#include <GTA/UI/Text.h>
#include <GTA/UI/Texture.h>

NAMESPACE_BEGIN;

class UIMenu;

class UIMenuItem
{
public:
	UIMenu* m_parent = nullptr;

	float m_height = 38;

	UIText m_strText;

	UITexture m_texHovering;

public:
	UIMenuItem(UIMenu* parent);
	~UIMenuItem();

	bool IsHovering();

	void Render(const glm::vec2 &pos);
};

NAMESPACE_END;

#pragma once

#include <Common.h>

#include <GTA/UI/MenuItem.h>

NAMESPACE_BEGIN;

class UIMenuItemCheckable : public UIMenuItem
{
public:
	bool m_checked = false;

public:
	UIMenuItemCheckable(UIMenu* parent);
	virtual ~UIMenuItemCheckable();

	virtual void Accept();

	virtual void Render(const glm::vec2 &pos);
};

NAMESPACE_END;

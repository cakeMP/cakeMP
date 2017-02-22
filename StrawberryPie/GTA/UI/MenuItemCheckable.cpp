#include <Common.h>

#include <GTA/UI/MenuItemCheckable.h>

#include <GTA/UI/Menu.h>

NAMESPACE_BEGIN;

UIMenuItemCheckable::UIMenuItemCheckable(UIMenu* parent)
	: UIMenuItem(parent)
{
}

UIMenuItemCheckable::~UIMenuItemCheckable()
{
}

void UIMenuItemCheckable::Accept()
{
	m_checked = !m_checked;

	UIMenuItem::Accept();
}

void UIMenuItemCheckable::Render(const glm::vec2 &pos)
{
	m_badgeRight = BadgeStyleNone;

	UIMenuItem::Render(pos);

	bool isHovering = IsHovering();

	if (m_checked) {
		m_texBadgeRight.Set(isHovering ? "shop_box_tickb" : "shop_box_tick");
	} else {
		m_texBadgeRight.Set(isHovering ? "shop_box_blankb" : "shop_box_blank");
	}
	m_texBadgeRight.Render(pos + glm::vec2(m_parent->m_width - m_height - 12, -6), glm::vec2(50));
}

NAMESPACE_END;

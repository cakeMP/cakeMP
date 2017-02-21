#include <Common.h>

#include <GTA/UI/MenuItem.h>

#include <GTA/UI/Menu.h>
#include <GTA/UI/UI.h>

NAMESPACE_BEGIN;

UIMenuItem::UIMenuItem(UIMenu* parent)
{
	m_parent = parent;

	m_strText.m_font = 0;
	m_strText.m_scale = 0.35f;

	m_texHovering.Set("commonmenu", "gradient_nav");
}

UIMenuItem::~UIMenuItem()
{
}

bool UIMenuItem::IsHovering()
{
	if (m_parent == nullptr) {
		return false;
	}
	return m_parent->SelectedItem() == this;
}

void UIMenuItem::Render(const glm::vec2 &pos)
{
	if (IsHovering()) {
		m_texHovering.Render(pos, glm::vec2(m_parent->m_width, m_height));
		m_strText.m_color = glm::vec4(0, 0, 0, 1);
	} else {
		m_strText.m_color = glm::vec4(1, 1, 1, 1);
	}

	m_strText.Render(pos + glm::vec2(10, 2));
}

NAMESPACE_END;

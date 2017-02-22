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

	m_texBadgeLeft.Set("commonmenu", "");
	m_texBadgeRight.Set("commonmenu", "");
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

void UIMenuItem::GoLeft()
{
}

void UIMenuItem::GoRight()
{
}

void UIMenuItem::Accept()
{
}

void UIMenuItem::Render(const glm::vec2 &pos)
{
	bool isHovering = IsHovering();

	if (isHovering) {
		m_texHovering.Render(pos, glm::vec2(m_parent->m_width, m_height));
		m_strText.m_color = glm::vec4(0, 0, 0, 1);
	} else {
		m_strText.m_color = glm::vec4(1, 1, 1, 1);
	}

	glm::vec2 textOffset = glm::vec2(10, 2);

	if (m_badgeLeft != BadgeStyleNone) {
		m_texBadgeLeft.Set(BadgeToTextureName(m_badgeLeft, isHovering));
		if (m_texBadgeLeft.IsValid()) {
			m_texBadgeLeft.Render(pos + glm::vec2(5, 0), glm::vec2(m_height));
			textOffset = glm::vec2(5 + m_height, 2);
		}
	}

	m_strText.Render(pos + textOffset);

	if (m_badgeRight != BadgeStyleNone) {
		m_texBadgeRight.Set(BadgeToTextureName(m_badgeRight, isHovering));
		if (m_texBadgeRight.IsValid()) {
			m_texBadgeRight.Render(pos + glm::vec2(m_parent->m_width - m_height - 5, 0), glm::vec2(m_height));
		}
	}
}

const char* UIMenuItem::BadgeToTextureName(UIMenuItemBadgeStyle style, bool hovering)
{
	switch (style) {
	case BadgeStyleNone: return "";

	case BadgeStyleBronzeMedal: return "mp_medal_bronze";
	case BadgeStyleSilverMedal: return "medal_silver";
	case BadgeStyleGoldMedal: return "mp_medal_gold";

	case BadgeStyleAlert: return "mp_alerttriangle";
	case BadgeStyleCrown: return "mp_hostcrown";

	case BadgeStyleAmmo: return hovering ? "shop_ammo_icon_b" : "shop_ammo_icon_a";
	case BadgeStyleArmour: return hovering ? "shop_armour_icon_b" : "shop_armour_icon_a";
	case BadgeStyleBarber: return hovering ? "shop_barber_icon_b" : "shop_barber_icon_a";
	case BadgeStyleClothes: return hovering ? "shop_clothing_icon_b" : "shop_clothing_icon_a";
	case BadgeStyleBike: return hovering ? "shop_garage_bike_icon_b" : "shop_garage_bike_icon_a";
	case BadgeStyleCar: return hovering ? "shop_garage_icon_b" : "shop_garage_icon_a";
	case BadgeStyleGun: return hovering ? "shop_gunclub_icon_b" : "shop_gunclub_icon_a";
	case BadgeStyleHeart: return hovering ? "shop_health_icon_b" : "shop_health_icon_a";
	case BadgeStyleLock: return "shop_lock";
	case BadgeStyleMakeup: return hovering ? "shop_makeup_icon_b" : "shop_makeup_icon_a";
	case BadgeStyleMask: return hovering ? "shop_mask_icon_b" : "shop_mask_icon_a";
	case BadgeStyleStar: return "shop_new_star";
	case BadgeStyleTattoo: return hovering ? "shop_tattoos_icon_b" : "shop_tattoos_icon_a";
	case BadgeStyleTick: return "shop_tick_icon";

	case BadgeStyleFranklin: return hovering ? "shop_franklin_icon_b" : "shop_franklin_icon_a";
	case BadgeStyleMichael: return hovering ? "shop_michael_icon_b" : "shop_michael_icon_a";
	case BadgeStyleTrevor: return hovering ? "shop_trevor_icon_b" : "shop_trevor_icon_a";
	}

	return "";
}

NAMESPACE_END;

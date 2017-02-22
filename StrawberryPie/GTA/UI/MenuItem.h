#pragma once

#include <Common.h>

#include <GTA/UI/Text.h>
#include <GTA/UI/Texture.h>

NAMESPACE_BEGIN;

class UIMenu;

enum UIMenuItemBadgeStyle
{
	BadgeStyleNone,

	BadgeStyleBronzeMedal,
	BadgeStyleSilverMedal,
	BadgeStyleGoldMedal,

	BadgeStyleAlert,
	BadgeStyleCrown,

	BadgeStyleAmmo,
	BadgeStyleArmour,
	BadgeStyleBarber,
	BadgeStyleClothes,
	BadgeStyleBike,
	BadgeStyleCar,
	BadgeStyleGun,
	BadgeStyleHeart,
	BadgeStyleLock,
	BadgeStyleMakeup,
	BadgeStyleMask,
	BadgeStyleStar,
	BadgeStyleTattoo,
	BadgeStyleTick,

	BadgeStyleFranklin,
	BadgeStyleMichael,
	BadgeStyleTrevor,
};

class UIMenuItem
{
public:
	UIMenu* m_parent = nullptr;

	float m_height = 38; //TODO: Make this const?

	UIText m_strText;
	UIText m_strTextRight;
	std::string m_description;

	UIMenuItemBadgeStyle m_badgeLeft = BadgeStyleNone;
	UIMenuItemBadgeStyle m_badgeRight = BadgeStyleNone;

	std::function<void()> m_onAccept;

protected:
	UITexture m_texHovering;

	UITexture m_texBadgeLeft;
	UITexture m_texBadgeRight;

public:
	UIMenuItem(UIMenu* parent);
	virtual ~UIMenuItem();

	virtual bool IsHovering();

	virtual void GoLeft();
	virtual void GoRight();
	virtual void Accept();

	virtual void Render(const glm::vec2 &pos);

public:
	static const char* BadgeToTextureName(UIMenuItemBadgeStyle style, bool hovering);
};

NAMESPACE_END;

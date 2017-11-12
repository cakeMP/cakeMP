#pragma once

#include <Common.h>

#include <GTA/UI/Text.h>
#include <GTA/UI/Texture.h>

NAMESPACE_BEGIN;

class UIMenuItem;
class UIMenuItemCheckable;

class UIMenu
{
public:
	bool m_visible = false;

	glm::vec2 m_origin;
	float m_width = 431.0f;
	int m_maxItemsVisible = 10;

	bool m_hasBanner = false;

	UIText m_strTitle;
	UIText m_strSubTitle;
	UIText m_strDescription;

	UITexture m_texBanner;
	UITexture m_texBackground;

private:
	std::vector<UIMenuItem*> m_items;
	int m_itemSelected = 0;
	int m_itemVisibleOffset = 0;

public:
	UIMenu();
	virtual ~UIMenu();

	virtual UIMenuItem* AddItem();
	virtual UIMenuItem* AddItem(const std::string &text);
	virtual UIMenuItemCheckable* AddItemCheckable();
	virtual UIMenuItemCheckable* AddItemCheckable(const std::string &text);
	virtual void DeleteItem(int index);
	virtual void DeleteItem(UIMenuItem* item);

	virtual void GoUp();
	virtual void GoDown();
	virtual void GoLeft();
	virtual void GoRight();
	virtual void GoAccept();
	virtual void GoBack();

	virtual void UpdateOffset();

	virtual UIMenuItem* SelectedItem();
	virtual int SelectedIndex();

	virtual float ContentsHeight();

	virtual void Update();
	virtual void Render();
};

NAMESPACE_END;

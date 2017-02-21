#pragma once

#include <Common.h>

#include <GTA/UI/Text.h>
#include <GTA/UI/Texture.h>

NAMESPACE_BEGIN;

class UIMenuItem;

class UIMenu
{
public:
	glm::vec2 m_origin;
	float m_width = 431.0f;

	bool m_hasBanner = false;

	UIText m_strTitle;
	UIText m_strSubTitle;

	UITexture m_texBanner;
	UITexture m_texBackground;

private:
	std::vector<UIMenuItem*> m_items;
	int m_itemSelected = 0;

public:
	UIMenu();
	~UIMenu();

	UIMenuItem* AddItem();
	UIMenuItem* AddItem(const std::string &text);
	void DeleteItem(int index);
	void DeleteItem(UIMenuItem* item);

	void GoUp();
	void GoDown();
	void GoLeft();
	void GoRight();

	UIMenuItem* SelectedItem();
	int SelectedIndex();

	float ContentsHeight();
	void Render();
};

NAMESPACE_END;

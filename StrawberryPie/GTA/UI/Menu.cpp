#include <Common.h>

#include <GTA/UI/Menu.h>

#include <GTA/UI/MenuItem.h>
#include <GTA/UI/UI.h>

#include <Enums/GameControl.h>

#include <shv/natives.h>

NAMESPACE_BEGIN;

UIMenu::UIMenu()
{
	m_strTitle.m_font = 1;
	m_strTitle.m_align = UITA_Center;

	m_strSubTitle.m_font = 0;
	m_strSubTitle.m_scale = 0.35f;
	m_strSubTitle.m_color = uiColor(glm::ivec4(93, 182, 229, 255));

	m_texBanner.Set("commonmenu", "interaction_bgd");
	m_texBackground.Set("commonmenu", "gradient_bgd");
}

UIMenu::~UIMenu()
{
	for (auto i : m_items) {
		delete i;
	}
}

UIMenuItem* UIMenu::AddItem()
{
	UIMenuItem* newItem = new UIMenuItem(this);
	m_items.push_back(newItem);
	return newItem;
}

UIMenuItem* UIMenu::AddItem(const std::string &text)
{
	UIMenuItem* newItem = AddItem();
	newItem->m_strText.m_text = text;
	return newItem;
}

void UIMenu::DeleteItem(int index)
{
	if (index < 0 || index >= m_items.size()) {
		assert(false);
		return;
	}
	delete m_items[index];
	m_items.erase(m_items.begin() + index);
}

void UIMenu::DeleteItem(UIMenuItem* item)
{
	auto it = std::find(m_items.begin(), m_items.end(), item);
	if (it != m_items.end()) {
		m_items.erase(it);
	}
	delete item;
}

void UIMenu::GoUp()
{
	m_itemSelected--;
	if (m_itemSelected < 0) {
		m_itemSelected = (int)m_items.size() - 1;
	}
	sndPlayFrontend("NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET");
}

void UIMenu::GoDown()
{
	m_itemSelected++;
	if (m_itemSelected >= m_items.size()) {
		m_itemSelected = 0;
	}
	sndPlayFrontend("NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET");
}

void UIMenu::GoLeft()
{
	//TODO
}

void UIMenu::GoRight()
{
	//TODO
}

UIMenuItem* UIMenu::SelectedItem()
{
	if (m_itemSelected < 0 || m_itemSelected >= m_items.size()) {
		return nullptr;
	}
	return m_items[m_itemSelected];
}

int UIMenu::SelectedIndex()
{
	return m_itemSelected;
}

float UIMenu::ContentsHeight()
{
	float ret = 0.0f;

	for (UIMenuItem* item : m_items) {
		ret += item->m_height;
	}

	return ret;
}

void UIMenu::Render()
{
	if (m_items.size() > 0) {
		if (CONTROLS::IS_CONTROL_JUST_PRESSED(0, GC_PhoneUp)) {
			GoUp();
		} else if (CONTROLS::IS_CONTROL_JUST_PRESSED(0, GC_PhoneDown)) {
			GoDown();
		} else if (CONTROLS::IS_CONTROL_JUST_PRESSED(0, GC_PhoneLeft)) {
			GoLeft();
		} else if (CONTROLS::IS_CONTROL_JUST_PRESSED(0, GC_PhoneRight)) {
			GoRight();
		}
	}

	glm::vec2 cursor = m_origin;

	if (m_hasBanner) {
		m_strTitle.Render(cursor + glm::vec2(m_width / 2.0f, 20));
		m_texBanner.Render(cursor, glm::vec2(m_width, 107));
		cursor += glm::vec2(0, 107);
	}

	m_strSubTitle.Render(cursor + glm::vec2(10, 2));
	uiDrawRectangle(cursor, glm::vec2(m_width, 37), glm::vec4(0, 0, 0, 1));
	cursor += glm::vec2(0, 37);

	m_texBackground.Render(cursor, glm::vec2(m_width, ContentsHeight()));

	for (int i = 0; i < m_items.size(); i++) {
		UIMenuItem* item = m_items[i];
		item->Render(cursor);
		cursor += glm::vec2(0, item->m_height);
	}
}

NAMESPACE_END;

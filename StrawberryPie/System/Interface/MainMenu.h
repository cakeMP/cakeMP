#pragma once

#include <Common.h>

#include <GTA/UI/Menu.h>
#include <GTA/UI/MenuItem.h>

NAMESPACE_BEGIN;

class MainMenu : public UIMenu
{
private:
	std::string m_connectIP = "127.0.0.1";
	uint16_t m_connectPort = 4499;

	UIMenuItem* m_itemConnectIP = nullptr;
	UIMenuItem* m_itemConnectPort = nullptr;
	UIMenuItem* m_itemConnect = nullptr;

	UIMenuItem* m_itemNickname = nullptr;

public:
	MainMenu();
	virtual ~MainMenu();

	void Initialize();

	virtual void Update();
	virtual void Render();
};

NAMESPACE_END;

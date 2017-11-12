#include <Common.h>

#include <System/Interface/MainMenu.h>

#include <System/Cake.h>
#include <GTA/UI/MenuItem.h>
#include <GTA/UI/UI.h>

#include <Enums/GameControl.h>

#include <Build.h>

#include <shv/natives.h>

NAMESPACE_BEGIN;

MainMenu::MainMenu()
{
	m_origin = glm::vec2(128, 256);
	m_hasBanner = true;
	m_strTitle.m_text = PROJECT_NAME;
	m_strSubTitle.m_text = PROJECT_BUILDTYPE;
}

MainMenu::~MainMenu()
{
}

void MainMenu::Initialize()
{
	m_connectIP = _pGame->m_settings.ConnectToHost;
	m_connectPort = _pGame->m_settings.ConnectToPort;

	m_itemConnectIP = AddItem("Connect IP");
	m_itemConnectIP->m_strTextRight.m_text = _pGame->m_settings.ConnectToHost;
	m_itemConnectIP->m_onAccept = [this]() {
		uiTextInput("IP", _pGame->m_settings.ConnectToHost, 30, [this](const std::string &text) {
			m_itemConnectIP->m_strTextRight.m_text = text;
			m_connectIP = text;
		});
	};

	m_itemConnectPort = AddItem("Connect Port");
	m_itemConnectPort->m_strTextRight.m_text = prsToString(_pGame->m_settings.ConnectToPort);
	m_itemConnectPort->m_onAccept = [this]() {
		uiTextInput("Port", prsToString(_pGame->m_settings.ConnectToPort), 30, [this](const std::string &text) {
			int num = prsToInteger(text);
			m_itemConnectPort->m_strTextRight.m_text = prsToString(num);
			m_connectPort = num;
		});
	};

	m_itemConnect = AddItem("Connect");
	m_itemConnect->m_onAccept = [this]() {
		if (_pGame->m_network.IsConnecting() || _pGame->m_network.IsConnected()) {
			_pGame->m_network.Disconnect();
		} else {
			_pGame->m_network.Connect(m_connectIP.c_str(), m_connectPort);
			m_visible = false;
		}
	};

	m_itemNickname = AddItem("Nickname");
	m_itemNickname->m_strTextRight.m_text = _pGame->m_player.m_nickname;
	m_itemNickname->m_onAccept = [this]() {
		uiTextInput("Nickname", _pGame->m_player.m_nickname, 30, [this](const std::string &text) {
			m_itemNickname->m_strTextRight.m_text = text;
			_pGame->m_player.m_nickname = text;
		});
	};
}

void MainMenu::GoBack()
{
	if (_pGame->m_network.IsConnected()) {
		UIMenu::GoBack();
	}
}

void MainMenu::Update()
{
	if (!m_visible) {
		return;
	}

	if (_pGame->m_network.IsConnecting() || _pGame->m_network.IsConnected()) {
		m_itemConnect->m_strText.m_text = "Disconnect";
	} else {
		m_itemConnect->m_strText.m_text = "Connect";
	}

	UIMenu::Update();
}

void MainMenu::Render()
{
	if (_pGame->m_network.IsConnected()) {
		if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(0, GC_FrontendPause) || CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(0, GC_FrontendPauseAlternate)) {
			if (!_pGame->m_interface.m_chat.IsFocused()) {
				m_visible = !m_visible;
				Update();
				return;
			}
		}
	}

	if (m_visible) {
		CONTROLS::DISABLE_ALL_CONTROL_ACTIONS(0);
	}

	UIMenu::Render();
}

NAMESPACE_END;

#include <Common.h>
#include <System/Interface/Interface.h>
#include <System/Strawberry.h>

#include <GTA/UI/UI.h>
#include <GTA/UI/MenuItem.h>

#include <Build.h>

#include <shv/natives.h>

NAMESPACE_BEGIN;

Interface::Interface()
{
}

Interface::~Interface()
{
	if (m_mainMenuCamera != nullptr) {
		delete m_mainMenuCamera;
	}
}

void Interface::Initialize()
{
	GRAPHICS::_GET_ACTIVE_SCREEN_RESOLUTION(&ui_screenWidth, &ui_screenHeight);
	logWrite("Resolution: %d x %d", ui_screenWidth, ui_screenHeight);

	m_mainMenuCamera = new ScriptCam;
	m_mainMenuCamera->SetPosition(glm::vec3(-538.535f, -1426.860f, 221.034f));
	m_mainMenuCamera->SetRotation(glm::vec3(0.0f, 0.0f, 340.907f));
	m_mainMenuCamera->SetActive();

	camScriptCams(true);

	m_mainMenu.Initialize();
	m_chat.Initialize();

	m_strVersion.m_font = 0;
	m_strVersion.m_scale = 0.3f;
	m_strVersion.m_align = UITA_Center;
	m_strVersion.m_color = glm::vec4(1, 1, 1, 0.4f);
	m_strVersion.m_text = PROJECT_NAME " " PROJECT_VERSION "~n~" PROJECT_BUILDTYPE " (" + _pGame->m_player.m_username + ")";

	m_mainMenu.m_visible = true;
}

void Interface::Update(float dt)
{
	m_mainMenu.Update();

	if (_pGame->m_network.IsConnected()) {
		m_chat.Update();
	}
}

void Interface::Render()
{
	m_mainMenu.Render();

	if (_pGame->m_network.IsConnected()) {
		m_chat.Render();
	} else {
		UI::HIDE_HUD_AND_RADAR_THIS_FRAME();
	}

	m_fpsCounter.Render();

	m_strVersion.Render(glm::vec2(ui_screenWidth / 2.0f, 0));
}

void Interface::OnConnected()
{
	uiNotify("~g~Connected");

	camScriptCams(false);
}

void Interface::OnDisconnected()
{
	m_chat.Clear();

	uiNotify("~r~Disconnected");

	camScriptCams(true);
}

void Interface::OnKeyDown(uint32_t key)
{
	m_chat.OnKeyDown(key);
}

NAMESPACE_END;

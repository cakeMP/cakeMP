#include <Common.h>

#include <System/Strawberry.h>

#include <Scripts/CleanWorld.h>
#include <Scripts/Game.h>

#include <GTA/UI/UI.h>

#include <Build.h>

#include <shv/main.h>
#include <shv/natives.h>

NAMESPACE_BEGIN;

Strawberry* _pGame = nullptr;

/*
static BOOL CALLBACK _windowEnumHandler(HWND hwnd, LPARAM lparam)
{
	Strawberry* pGame = (Strawberry*)lparam;

	DWORD pid = 0;
	GetWindowThreadProcessId(hwnd, &pid);
	if (pid == GetCurrentProcessId()) {
		pGame->m_hWnd = hwnd;
		return FALSE;
	}

	return TRUE;
}
*/

Strawberry::Strawberry(HMODULE hInstance)
{
	m_hInstance = hInstance;

	memset(m_keyStates, 0, sizeof(m_keyStates));

	scriptRegister(hInstance, scriptCleanWorld);
	scriptRegister(hInstance, scriptGame);
}

Strawberry::~Strawberry()
{
	scriptUnregister(m_hInstance);
}

void Strawberry::Initialize()
{
	logWrite("Client initializing.");

	/*
	EnumWindows(_windowEnumHandler, (LPARAM)this);
	logWrite("Window handle: %p", m_hWnd);
	*/

	PED::ADD_RELATIONSHIP_GROUP("SYNCPED", (Hash*)&m_pedRelGroup);

	GAMEPLAY::SET_WEATHER_TYPE_NOW_PERSIST("CLEAR");
	TIME::SET_CLOCK_TIME(12, 0, 0);
	TIME::PAUSE_CLOCK(true);

	m_player.Initialize();
	m_network.Initialize();
	m_interface.Initialize();
}

void Strawberry::Update(float dt)
{
	m_gameTime = GAMEPLAY::GET_GAME_TIMER();

	m_interface.Update(dt);

	m_player.Update();
	m_network.Update();

	if (uiTextInputVisible()) {
		uiTextInputUpdate();
	}
}

void Strawberry::Render()
{
	/*
	//TODO: This should be an option (it slows down keyboard input outside of the game sadly)
	if (GetForegroundWindow() != m_hWnd) {
		Sleep(17);
	}
	*/

	m_interface.Render();

	m_player.Render();
}

void Strawberry::OnConnected()
{
	m_interface.OnConnected();
}

void Strawberry::OnDisconnected()
{
	m_interface.OnDisconnected();

	//TODO: Also delete local entities (m_entitiesLocal in Strawberry?)
	m_network.ClearEntities();
}

void Strawberry::OnKeyDown(uint32_t key)
{
	assert(key < 256);
	if (key >= 256) {
		logWrite("OnKeyDown out of range: %u", key);
		return;
	}

	m_keyStates[key] = 1;

	if (key == VK_F3) {
		m_network.Connect(m_settings.ConnectToHost.c_str(), m_settings.ConnectToPort);
	} else if (key == VK_F4) {
		m_network.Disconnect();
	} else if (key == VK_F5) {
		int veh = PED::GET_VEHICLE_PED_IS_IN(m_player.GetLocalHandle(), false);
		if (veh > 0) {
			VEHICLE::SET_VEHICLE_FIXED(veh);
		}
	}

	if (m_network.IsConnected()) {
		m_interface.OnKeyDown(key);
	}
}

void Strawberry::OnKeyUp(uint32_t key)
{
	assert(key < 256);
	if (key >= 256) {
		logWrite("OnKeyUp out of range: %u", key);
		return;
	}

	m_keyStates[key] = 0;
}

bool Strawberry::IsKeyDown(uint8_t key)
{
	return (m_keyStates[key] == 1);
}

NAMESPACE_END;

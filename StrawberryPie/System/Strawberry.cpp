#include <Common.h>

#include <System/Strawberry.h>

#include <Scripts/CleanWorld.h>
#include <Scripts/Game.h>

#include <shv/main.h>

NAMESPACE_BEGIN;

Strawberry* _pGame = nullptr;

Strawberry::Strawberry(HMODULE hInstance)
{
	m_hInstance = hInstance;

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

	m_player.Initialize();
	m_network.Initialize();
}

void Strawberry::Update()
{
	m_player.Update();
	m_network.Update();
}

void Strawberry::OnKeyDown(uint32_t key)
{
	if (key == VK_F3) {
		m_network.Connect(m_settings.ConnectToHost.c_str(), m_settings.ConnectToPort);
	} else if (key == VK_F4) {
		m_network.Disconnect();
	}
}

void Strawberry::OnKeyUp(uint32_t key)
{
}

NAMESPACE_END;

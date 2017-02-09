#include <Common.h>

#include <Strawberry.h>

#include <Scripts/CleanWorld.h>
#include <Scripts/Game.h>

#include <shv/main.h>

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

void Strawberry::Update()
{
	m_network.Update();
}

void Strawberry::OnKeyDown(uint32_t key)
{
	if (key == VK_F3) {
		m_network.Connect("localhost", 4499);
	} else if (key == VK_F4) {
		m_network.Disconnect();
	}
}

void Strawberry::OnKeyUp(uint32_t key)
{
}

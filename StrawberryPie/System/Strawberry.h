#pragma once

#include <Network/NetworkManager.h>

#include <System/Settings.h>
#include <System/Chat.h>

class Strawberry
{
public:
	HMODULE m_hInstance;

	NetworkManager m_network;
	Settings m_settings;

	Chat m_chat;

public:
	Strawberry(HMODULE hInstance);
	~Strawberry();

	void Initialize();
	void Update();

	void OnKeyDown(uint32_t key);
	void OnKeyUp(uint32_t key);
};

extern Strawberry* _pGame;

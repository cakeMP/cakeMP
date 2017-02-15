#pragma once

#include <Common.h>

#include <Network/NetworkManager.h>

#include <System/Settings.h>
#include <System/Chat.h>

#include <Entities/LocalPlayer.h>

NAMESPACE_BEGIN;

class Strawberry
{
public:
	HMODULE m_hInstance;

	NetworkManager m_network;
	Settings m_settings;

	LocalPlayer m_player;

	Chat m_chat;

	uint32_t m_pedRelGroup = 0;

public:
	Strawberry(HMODULE hInstance);
	~Strawberry();

	void Initialize();
	void Update();

	void OnKeyDown(uint32_t key);
	void OnKeyUp(uint32_t key);
};

extern Strawberry* _pGame;

NAMESPACE_END;

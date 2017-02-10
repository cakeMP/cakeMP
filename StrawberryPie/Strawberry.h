#pragma once

#include <Network/NetworkManager.h>
#include <Chat.h>

class Strawberry
{
public:
	HMODULE m_hInstance;

	NetworkManager m_network;
	Chat m_chat;

public:
	Strawberry(HMODULE hInstance);
	~Strawberry();

	void Update();

	void OnKeyDown(uint32_t key);
	void OnKeyUp(uint32_t key);
};

extern Strawberry* _pGame;

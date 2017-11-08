#pragma once

#include <Common.h>

#include <System/Interface/MainMenu.h>
#include <System/Interface/Chat.h>
#include <System/Interface/FPSCounter.h>

#include <GTA/ScriptCam.h>

NAMESPACE_BEGIN;

struct LogLine
{
	std::string m_text;
	float m_ttl;
};

class Interface
{
public:
	MainMenu m_mainMenu;
	ScriptCam* m_mainMenuCamera = nullptr;

	Chat m_chat;
	FPSCounter m_fpsCounter;
	UIText m_strVersion;
	UIText m_strDebug;

public:
	Interface();
	~Interface();

	void Initialize();
	void Update(float dt);
	void Render();

	void OnConnected();
	void OnDisconnected();

	void OnKeyDown(uint32_t key);
};

NAMESPACE_END;

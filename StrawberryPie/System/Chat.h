#pragma once

#include <Common.h>

#include <GTA/Scaleform.h>

NAMESPACE_BEGIN;

class Chat
{
private:
	Scaleform m_scaleform;
	bool m_focused = false;
	std::string m_currentInput;

public:
	Chat();
	~Chat();

	void Initialize();
	void Update();
	void Render();

	void OnKeyDown(uint32_t c);

	void SetFocused(bool focus);
	bool IsFocused();

	void AddMessage(const char* sender, const char* message);
	void Clear();
};

NAMESPACE_END;

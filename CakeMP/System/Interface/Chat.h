#pragma once

#include <Common.h>

#include <GTA/UI/Text.h>

NAMESPACE_BEGIN;

class Chat
{
private:
	bool m_focused = false;
	std::string m_currentInput;

	float m_width = 300;
	float m_height = 230;
	int m_maxLines = 10;

	std::vector<UIText> m_chatLines;
	UIText m_chatInput;

public:
	Chat();
	~Chat();

	void Initialize();
	void Update(float dt);
	void Render();

	void OnKeyDown(uint32_t c);

	void SetFocused(bool focus);
	bool IsFocused();

	void AddMessage(const std::string &message);
	void Clear();
};

NAMESPACE_END;

#pragma once

#include <Common.h>

class Chat
{
public:
	Chat();
	~Chat();

	void Update();

	void FocusChat();
	void AddText(const std::string &str);
};

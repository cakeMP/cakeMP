#pragma once

#include <Common.h>

//TODO: This file should be somewhere else!

class Chat
{
public:
	Chat();
	~Chat();

	void Update();

	void FocusChat();
	void AddText(const std::string &str);
};

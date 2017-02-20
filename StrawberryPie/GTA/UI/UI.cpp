#include <Common.h>

#include <GTA/UI/UI.h>

#include <shv/natives.h>

NAMESPACE_BEGIN;

int ui_screenWidth = 0;
int ui_screenHeight = 0;

void uiAddLongString(const char* str)
{
	size_t len = strlen(str);

	if (len <= 99) {
		UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(str);
		return;
	}

	char buffer[100];
	for (size_t i = 0; i < len; i += 99) {
		const char* p = str + i;

		size_t sublen = strlen(p);
		if (sublen > 99) {
			sublen = 99;
		}

		memcpy(buffer, p, sublen);
		buffer[sublen] = '\0';

		UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(buffer);
	}
}

void uiNotify(const char* str)
{
	UI::_SET_NOTIFICATION_TEXT_ENTRY("CELL_EMAIL_BCON");
	uiAddLongString(str);
	UI::_DRAW_NOTIFICATION(false, true);
}

void uiNotify(const std::string &str)
{
	uiNotify(str.c_str());
}

NAMESPACE_END;

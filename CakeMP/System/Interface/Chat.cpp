#include <Common.h>

#include <System/Interface/Chat.h>
#include <System/Strawberry.h>

#include <GTA/UI/UI.h>

#include <shv/main.h>
#include <shv/natives.h>

NAMESPACE_BEGIN;

#include <Windows.h> //Winuser.h

Chat::Chat()
{
}

Chat::~Chat()
{
}

void Chat::Initialize()
{
	m_chatInput.m_font = 0;
	m_chatInput.m_scale = 0.35f;
	m_chatInput.m_outline = true;
}

void Chat::Update(float dt)
{
	//
}

void Chat::Render()
{
	glm::vec2 cursorPos = glm::vec2(20, 128 + m_height);

	if (IsFocused()) {
		glm::vec2 textSize = m_chatInput.Measure();
		if (textSize.x < 50) {
			textSize.x = 50;
		}
		uiDrawRectangle(cursorPos, textSize, glm::vec4(0.2f, 0.2f, 0.2f, 0.7f));
		m_chatInput.Render(cursorPos);
	}

	for (int i = (int)m_chatLines.size() - 1; i >= 0; i--) {
		UIText &chatLine = m_chatLines[i];
		chatLine.m_font = 0;
		chatLine.m_scale = 0.35f;
		chatLine.m_outline = true;
		cursorPos.y -= chatLine.Measure().y;
		chatLine.Render(cursorPos);
	}

	//TODO: This could go into Update() if we write a controls helper class
	if (IsFocused()) {
		CONTROLS::DISABLE_ALL_CONTROL_ACTIONS(0);
	}
}

void Chat::OnKeyDown(uint32_t c)
{
	if (c == 'T' && !IsFocused()) {
		SetFocused(true);
		return;
	}

	if (!IsFocused()) {
		return;
	}

	if (c == VK_RETURN) {
		NetworkMessage* msgChat = new NetworkMessage(NMT_ChatMessage);
		msgChat->Write(m_currentInput);
		_pGame->m_network.SendToHost(msgChat);

		m_currentInput = "";
		m_chatInput.m_text = m_currentInput;
		//m_scaleform.CallFunction("COMPLETE_TEXT");
		SetFocused(false);
		return;

	} else if (c == VK_BACK) {
		if (m_currentInput.size() > 0) {
			m_currentInput.pop_back();
			m_chatInput.m_text = m_currentInput;
			//m_scaleform.CallFunction("DELETE_TEXT");
		}
		return;

	} else if (c == VK_ESCAPE) {
		m_currentInput = "";
		m_chatInput.m_text = m_currentInput;
		//m_scaleform.CallFunction("ABORT_TEXT");
		SetFocused(false);
		return;

	} else if (c == VK_PRIOR) {
		//m_scaleform.CallFunction("PAGE_UP");
		return;

	} else if (c == VK_NEXT) {
		//m_scaleform.CallFunction("PAGE_DOWN");
		return;
	}

	if (c <= 255) {
		wchar_t wcharBuffer[10];

		unsigned char keystate[256]; //using _pGame->m_keyStates does not work
		GetKeyboardState(keystate);

		auto keyboardLayout = GetKeyboardLayout(GetCurrentThreadId());
		int scanCodeEx = MapVirtualKeyExW(c, MAPVK_VK_TO_VSC_EX, keyboardLayout);
		int ret = ToUnicodeEx(c, scanCodeEx, keystate, wcharBuffer, 10, 0, keyboardLayout);

		if (ret < 1) return; //invalid key or dead key. Dead keys should get proper handling tho

		char *buffer = (char *)malloc(ret + 1);
		wcstombs(buffer, wcharBuffer, ret);
		buffer[ret] = '\0';

		m_currentInput += buffer;
		m_chatInput.m_text = m_currentInput;

		/*
		m_scaleform.StartFunction("ADD_TEXT");
		m_scaleform.PushParam(buffer);
		m_scaleform.FinishFunction();
		*/

		free(buffer);
	}
}

void Chat::SetFocused(bool focus)
{
	/*
	if (focus != m_focused) {
		m_scaleform.StartFunction("SET_FOCUS");
		if (focus) {
			m_scaleform.PushParam(2);
		} else if (!focus) {
			m_scaleform.PushParam(1);
		}
		m_scaleform.PushParam(0);
		m_scaleform.PushParam("");
		m_scaleform.PushParam(_pGame->m_player.m_nickname);
		m_scaleform.FinishFunction();
	}
	*/

	m_focused = focus;
}

bool Chat::IsFocused()
{
	return m_focused;
}

void Chat::AddMessage(const std::string &message)
{
	m_chatLines.push_back(UIText(message));

	if ((int)m_chatLines.size() > m_maxLines) {
		m_chatLines.erase(m_chatLines.begin(), m_chatLines.begin() + (m_chatLines.size() - m_maxLines));
	}
}

void Chat::Clear()
{
	m_chatLines.clear();
}

NAMESPACE_END;

#include <Common.h>

#include <System/Interface/Chat.h>
#include <System/Strawberry.h>

#include <shv/main.h>
#include <shv/natives.h>

NAMESPACE_BEGIN;

#include <windows.h> //Winuser.h

Chat::Chat()
{
}

Chat::~Chat()
{
}

void Chat::Initialize()
{
	m_scaleform.Load("multiplayer_chat");
	// Visible states:
	//  * VISIBLE_STATE_HIDDEN: 0
	//  * VISIBLE_STATE_DEFAULT: 1
	//  * VISIBLE_STATE_TYPING: 2
	// Focus modes:
	//  * FOCUS_MODE_LOBBY: 0
	//  * FOCUS_MODE_GAME: 1
	// Default display config:
	//  * screenWidth: 1280
	//  * screenHeight: 720
	//  * safeTop: 0.5
	//  * safeBottom: 0.95
	//  * safeLeft: 0.05
	//  * safeRight: 0.95
	//  * isWideScreen: true
	//  * isCircleAccept: false
	//  * isHiDef: true
	//  * isAsian: false
	// SET_DISPLAY_CONFIG(_screenWidthPixels, _screenHeightPixels, _safeTopPercent, _safeBottomPercent, _safeLeftPercent, _safeRightPercent, _isWideScreen, _isCircleAccept)
	// ADD_TEXT(text)
	// DELETE_TEXT()
	// ABORT_TEXT()
	// COMPLETE_TEXT()
	// SET_TYPING_DONE()
	// ADD_MESSAGE(player, message, scope, teamOnly, eHudColour)
	// SET_FOCUS(eVisibleState, scopeType, scope, player, eHudColour)
	// SET_FOCUS_MODE(focusMode)
	// PAGE_UP()
	// PAGE_DOWN()
	// RESET()
}

void Chat::Update()
{
}

void Chat::Render()
{
	m_scaleform.Render();

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
		m_scaleform.CallFunction("COMPLETE_TEXT");
		SetFocused(false);
		return;

	} else if (c == VK_BACK) {
		if (m_currentInput.size() > 0) {
			m_currentInput.pop_back();
			m_scaleform.CallFunction("DELETE_TEXT");
		}
		return;

	} else if (c == VK_ESCAPE) {
		m_currentInput = "";
		m_scaleform.CallFunction("ABORT_TEXT");
		SetFocused(false);
		return;

	} else if (c == VK_PRIOR) {
		m_scaleform.CallFunction("PAGE_UP");
		return;

	} else if (c == VK_NEXT) {
		m_scaleform.CallFunction("PAGE_DOWN");
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

		m_scaleform.StartFunction("ADD_TEXT");
		m_scaleform.PushParam(buffer);
		m_scaleform.FinishFunction();

		free(buffer);
	}
}

void Chat::SetFocused(bool focus)
{
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

	m_focused = focus;
}

bool Chat::IsFocused()
{
	return m_focused;
}

void Chat::AddMessage(const char* sender, const char* message)
{
	m_scaleform.StartFunction("ADD_MESSAGE");
	m_scaleform.PushParam(sender);
	m_scaleform.PushParam(message);
	m_scaleform.FinishFunction();
}

void Chat::Clear()
{
	m_scaleform.CallFunction("RESET");
}

NAMESPACE_END;

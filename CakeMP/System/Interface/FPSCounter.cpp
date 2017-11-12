#include <Common.h>

#include <System/Interface/FPSCounter.h>

#include <GTA/UI/UI.h>

#include <shv/natives.h>

NAMESPACE_BEGIN;

FPSCounter::FPSCounter()
{
	m_text.m_font = 0;
	m_text.m_outline = true;
	m_text.m_align = UITA_Right;
	m_text.m_scale = 0.25f;
}

FPSCounter::~FPSCounter()
{
}

void FPSCounter::Render()
{
	static ClockTime _tmLastFrame = Clock::now();

	ClockTime tmNow = Clock::now();
	auto tmDuration = MicroDuration(tmNow - _tmLastFrame);
	_tmLastFrame = Clock::now();

	float tmSpent = (tmDuration.count() / 1000.0f);
	float currentFPS = 1000.0f / max(1.0f, tmSpent);
	m_framerate.Add(currentFPS);

	char buffer[64];
	sprintf(buffer, "~r~%d ~w~&lt; %d &lt; ~g~%d ~w~FPS", (int)m_framerate.Min(), (int)m_framerate.Value(), (int)m_framerate.Max());

	m_text.m_text = buffer;
	m_text.Render(glm::vec2(ui_screenWidth - 4, 4));
}

NAMESPACE_END;

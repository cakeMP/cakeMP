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

	for (int i = 0; i < NumFrameAvg; i++) {
		m_lastFrames[i] = 0.0f;
	}
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
	memmove(m_lastFrames, m_lastFrames + 1, sizeof(m_lastFrames) - sizeof(float));
	m_lastFrames[NumFrameAvg - 1] = currentFPS;

	float fps = 0.0f;
	for (int i = 0; i < NumFrameAvg; i++) {
		fps += m_lastFrames[i];
	}

	fps /= (float)NumFrameAvg;

	char buffer[64];
	sprintf(buffer, "%d FPS", (int)fps);

	m_text.m_text = buffer;
	m_text.Render(glm::vec2(ui_screenWidth - 4, 4));
}

NAMESPACE_END;

#pragma once

#include <Common.h>

#include <GTA/UI/Text.h>

NAMESPACE_BEGIN;

class FPSCounter
{
private:
	const static int NumFrameAvg = 60;

	UIText m_text;
	float m_lastFrames[NumFrameAvg];

public:
	FPSCounter();
	~FPSCounter();

	void Render();
};

NAMESPACE_END;

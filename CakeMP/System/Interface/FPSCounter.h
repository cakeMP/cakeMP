#pragma once

#include <Common.h>

#include <GTA/UI/Text.h>

#include <Utils/Average.h>

NAMESPACE_BEGIN;

class FPSCounter
{
private:
	UIText m_text;
	Average<float> m_framerate;

public:
	FPSCounter();
	~FPSCounter();

	void Render();
};

NAMESPACE_END;

#pragma once

#include <Common.h>

#include <System/Entity.h>

NAMESPACE_BEGIN;

class Ped : public Entity
{
public:
	Ped();
	virtual ~Ped();

	virtual void SetModel(uint32_t hash);
};

NAMESPACE_END;

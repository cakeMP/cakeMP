#pragma once

#include <World.h>

class WorldQueryRange : public Octree<WorldNode>::Callback
{
public:
	glm::vec3 m_pos;
	float m_range = 0.0f;
	float m_rangeSquared = 0.0f;
	std::vector<Entity*> m_entities;

public:
	WorldQueryRange(const glm::vec3 &pos, float range);

	virtual bool operator()(const glm::vec3 &min, const glm::vec3 &max, WorldNode &node);
};

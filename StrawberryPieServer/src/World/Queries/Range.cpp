#include "..\..\Common.h"

#include "Range.h"

WorldQueryRange::WorldQueryRange(const glm::vec3 &pos, float range)
{
	m_pos = pos;
	m_range = range;
	m_rangeSquared = m_range * m_range;
}

bool WorldQueryRange::operator()(const glm::vec3 &min, const glm::vec3 &max, WorldNode &node)
{
	float cellSize = glm::distance(min, max);
	float maxDist = cellSize * 0.5f + m_range;

	glm::vec3 vCenter = (min + max) * 0.5f;
	if (glm::distance2(m_pos, vCenter) > maxDist * maxDist) {
		return false;
	}

	for (Entity* ent : node.m_entities) {
		if (glm::distance2(ent->GetPosition(), m_pos) < m_rangeSquared) {
			m_entities.push_back(ent);
		}
	}

	return true;
}

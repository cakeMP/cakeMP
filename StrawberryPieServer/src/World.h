#pragma once

#include <Common.h>
#include <Entity.h>

#include <Octree.h>

class WorldNode
{
public:
	std::vector<Entity*> m_entities;
};

class World
{
public:
	Octree<WorldNode> m_entities;
	std::unordered_map<uint32_t, Entity*> m_allEntities;

public:
	World();
	~World();

	void Delete();

	void AddToOctree(Entity* ent);
	void RemoveFromOctree(Entity* ent);
	void RebuildOctree();

	void QueryRange(const glm::vec3 &pos, float range, std::vector<Entity*> &out);

	void AddEntity(Entity* ent);
	void RemoveEntity(Entity* ent);
	void EntityMoved(Entity* ent, const glm::vec3 &oldPos);

	void Update();
};

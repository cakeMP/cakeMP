#include <Common.h>

#include <World/World.h>
#include <World/Queries/Range.h>

#include <GameServer.h>

#include <Entities/Vehicle.h>

World::World()
	: m_entities(
			//TODO: Make cell size configurable in settings
			glm::vec3(-4000.0f, -4000.0f, -1000.0f), // Min
			glm::vec3(6000.0f, 8000.0f, 10000.0f), // Max
			glm::vec3(200.0f, 200.0f, 200.0f) // Cell Size
		)
{
}

World::~World()
{
	Delete();
}

void World::Initialize()
{
	logWrite("Initializing world...");

	Vehicle* newVehicle = new Vehicle(_pServer->m_network.AssignHandle());
	AddEntity(newVehicle);

	newVehicle->SetModel(hashGet("asea"));
	newVehicle->SetPosition(glm::vec3(-4.31804f, 546.8f, 173.75f));

	logWrite("Initialized.");
}

void World::Delete()
{
	m_entities.clear();
	for (auto &pair : m_allEntities) {
		pair.second->ReleaseForce();
	}
	m_allEntities.clear();
}

void World::AddToOctree(Entity* ent)
{
	//TODO: Use cached WorldNode value to assert we're not already added to a node
	WorldNode &node = m_entities.getCell(ent->GetPosition());
	node.m_entities.push_back(ent);
}

void World::RemoveFromOctree(Entity* ent)
{
	//TODO: Cache the WorldNode inside of the entity so we can fetch it easily without having to traverse
	WorldNode &node = m_entities.getCell(ent->GetPosition());
	auto it = std::find(node.m_entities.begin(), node.m_entities.end(), ent);
	if (it == node.m_entities.end()) {
		assert(false);
		return;
	}
	node.m_entities.erase(it);
}

void World::RebuildOctree()
{
	//TODO: Clear cached WorldNode value before clearing and re-adding
	m_entities.clear();
	for (auto &pair : m_allEntities) {
		AddToOctree(pair.second);
	}
}

void World::QueryRange(const glm::vec3 &pos, float range, std::vector<Entity*> &out, Entity* except)
{
	WorldQueryRange query(pos, range);
	m_entities.traverse(query);
	for (Entity* ent : query.m_entities) {
		auto insertPos = std::lower_bound(out.begin(), out.end(), ent);
		if (except != nullptr && ent == except) {
			continue;
		}
		out.insert(insertPos, ent);
	}
}

void World::AddEntity(Entity* ent)
{
	m_allEntities[ent->m_handle] = ent;
	AddToOctree(ent);
}

void World::RemoveEntity(Entity* ent)
{
	RemoveFromOctree(ent);
	auto it = m_allEntities.find(ent->m_handle);
	if (it == m_allEntities.end()) {
		assert(false);
		return;
	}
	m_allEntities.erase(it);
}

Entity* World::GetEntityFromHandle(const NetHandle &handle)
{
	auto it = m_allEntities.find(handle);
	if (it == m_allEntities.end()) {
		return nullptr;
	}
	return it->second;
}

void World::EntityMoved(Entity* ent, const glm::vec3 &oldPos)
{
	WorldNode &oldNode = m_entities.getCell(oldPos); //TODO: Use cached value instead!
	WorldNode &newNode = m_entities.getCell(ent->GetPosition());

	// We're still in the same node, no need to switch
	if (&oldNode == &newNode) {
		return;
	}

	auto it = std::find(oldNode.m_entities.begin(), oldNode.m_entities.end(), ent);
	if (it == oldNode.m_entities.end()) {
		assert(false);
	} else {
		oldNode.m_entities.erase(it);
	}

	newNode.m_entities.push_back(ent);
}

void World::Update()
{
	for (auto &pair : m_allEntities) {
		pair.second->Update();
	}
}

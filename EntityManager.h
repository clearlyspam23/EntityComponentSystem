#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "Typedefs.h"
#include <list>
#include <set>
#include <memory>

class Entity;
class EntityTemplate;
class System;

class EntityManager
{
public:
	EntityManager() : _ID(0){}

	~EntityManager();

	std::shared_ptr<Entity> createEntity();

	std::shared_ptr<Entity> createEntity(const EntityTemplate& t);

	void modifyEntity(std::shared_ptr<Entity> e, const EntityTemplate& t);

	void removeEntity(std::shared_ptr<Entity> entity);

	const std::set<std::shared_ptr<Entity>>& getEntities() const { return _entities; }

	std::set<std::shared_ptr<Entity>> getEntitiesWithComponents(const COMPONENT_ID& components) const;

	void addSystem(System* system);

	void onTick(float delta);
private:
	EntityManager(const EntityManager& manager);
	EntityManager& operator=(EntityManager other);
	std::set<std::shared_ptr<Entity>> _entities;
	std::set<System*> _systems;
	ENTITY_ID _ID;
};

#endif //ENTITYMANAGER_H
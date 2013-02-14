#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "Typedefs.h"
#include "EntityTemplate.h"
#include <list>
#include <set>
#include <vector>
#include <memory>
#include <utility>

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

	template<typename T>
	T* getSystem();

	System* getSystem(unsigned int id);

	void onTick(float delta);
private:
	void _modifyEntity(std::shared_ptr<Entity> e, const EntityTemplate& t);
	void _removeEntity(std::shared_ptr<Entity> entity);
	void _createEntity(std::shared_ptr<Entity>& entity);
	EntityManager(const EntityManager& manager);
	EntityManager& operator=(EntityManager other);
	std::set<std::shared_ptr<Entity>> _entities;
	std::vector<System*> _systems;
	std::list<std::shared_ptr<Entity>> toBeAdded;
	std::list<std::shared_ptr<Entity>> toBeRemoved;
	std::list<std::pair<std::shared_ptr<Entity>, EntityTemplate>> toBeModified;
	ENTITY_ID _ID;
};

template <typename T>
T* EntityManager::getSystem()
{
	return (T*) getSystem(T::SYSTEMID());
}

#endif //ENTITYMANAGER_H
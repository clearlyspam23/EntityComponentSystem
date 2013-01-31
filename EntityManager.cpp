#include "EntityManager.h"
#include "Entity.h"
#include "EntityTemplate.h"
#include "System.h"
#include "Component.h"
#include <algorithm>


using namespace std;

EntityManager::~EntityManager()
{
	for(auto i = _systems.begin(); i != _systems.end(); i++)
	{
		delete *i;
	}
}

shared_ptr<Entity> EntityManager::createEntity()
{
	shared_ptr<Entity> e = shared_ptr<Entity>(new Entity);
	e->setId(_ID++);
	_entities.insert(e);
	return e;
}

shared_ptr<Entity> EntityManager::createEntity(const EntityTemplate& t)
{
	shared_ptr<Entity> e = shared_ptr<Entity>(new Entity);
	e->setId(_ID++);
	t.constructEntity(e);
	for(System* s : _systems)
	{
		if(s->matchesRequirements(e))
			s->onEntityAdd(e);
	}
	_entities.insert(e);
	return e;
}

void EntityManager::modifyEntity(shared_ptr<Entity> entity, const EntityTemplate& t)
{
	std::set<System*> currentSystems;
	for(System* s : _systems)
	{
		if(s->matchesRequirements(entity))
			currentSystems.insert(s);
	}
	t.reconstructEntity(entity);
	 for(System* s : _systems)
	 {
		 if(s->matchesRequirements(entity)&&currentSystems.find(s)==currentSystems.end())
		 {
			 s->onEntityAdd(entity);
		 }
		 else if(currentSystems.find(s)!=currentSystems.end()&&!s->matchesRequirements(entity))
		 {
			 s->onEntityRemove(entity);
		 }
	 }
}

void EntityManager::removeEntity(shared_ptr<Entity> entity)
{
	for(System* s : _systems)
	{
		s->onEntityRemove(entity);
	}
	_entities.erase(entity);
}

set<shared_ptr<Entity>> EntityManager::getEntitiesWithComponents(const COMPONENT_ID& components) const
{
	set<shared_ptr<Entity>> set;
	for(shared_ptr<Entity> e : _entities)
	{
		if((e->getIncludedComponents()&components)==components)
		{
			set.insert(e);
		}
	}
	return set;
}

void  EntityManager::addSystem(System* system)
{
	_systems.insert(system);
}

void EntityManager::onTick(float delta)
{
	for(auto i = _systems.begin(); i != _systems.end(); i++)
	{
		(*i)->onTick(delta, *this);
	}
}
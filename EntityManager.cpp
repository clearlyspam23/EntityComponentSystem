#include "EntityManager.h"
#include "Entity.h"
#include "System.h"
#include "Component.h"
#include <algorithm>


using namespace std;


EntityManager::~EntityManager()
{
	for(System* s : _systems)
	{
		delete s;
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
	toBeAdded.push_back(e);
	return e;
}

void EntityManager::modifyEntity(shared_ptr<Entity> entity, const EntityTemplate& t)
{
	toBeModified.push_back(pair<std::shared_ptr<Entity>, EntityTemplate>(entity, t));
}

void EntityManager::removeEntity(shared_ptr<Entity> entity)
{
	toBeRemoved.push_back(entity);
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
	if(_systems.size()<=system->systemId())
		_systems.resize(system->systemId()+1, nullptr);
	else if(_systems[system->systemId()]!=nullptr)
		delete _systems[system->systemId()];
	_systems[system->systemId()] = system;
}

System* EntityManager::getSystem(unsigned int id)
{
	return _systems[id];
}

void EntityManager::onTick(float delta)
{
	for(;toBeAdded.size()>0;toBeAdded.pop_front())
	{
		std::shared_ptr<Entity> e = toBeAdded.front();
		_createEntity(e);
	}
	for(System* s : _systems)
	{
		if(s!=nullptr)
			s->onTick(delta, *this);
	}
	for(;toBeRemoved.size()>0;toBeRemoved.pop_front())
	{
		std::shared_ptr<Entity> entity = toBeRemoved.front();
		_removeEntity(entity);
	}
	for(;toBeModified.size()>0;toBeModified.pop_front())
	{
		pair<shared_ptr<Entity>, EntityTemplate> t = toBeModified.front();
		_modifyEntity(t.first, t.second);
	}
}

void EntityManager::_createEntity(std::shared_ptr<Entity>& e)
{
	for(System* s : _systems)
	{
		if(s!=nullptr&&s->matchesRequirements(e))
			s->onEntityAdd(e);
	}
	_entities.insert(e);
}

void EntityManager::_removeEntity(std::shared_ptr<Entity> entity)
{
	for(System* s : _systems)
	{
		if(s!=nullptr&&s->matchesRequirements(entity))
			s->onEntityRemove(entity);
	}
	_entities.erase(entity);
}

void EntityManager::_modifyEntity(std::shared_ptr<Entity> entity, const EntityTemplate& t)
{
	std::set<System*> currentSystems;
	for(System* s : _systems)
	{
		if(s!=nullptr&&s->matchesRequirements(entity))
			currentSystems.insert(s);
	}
	t.reconstructEntity(entity);
	 for(System* s : _systems)
	 {
		 if(s!=nullptr&&s->matchesRequirements(entity)&&currentSystems.find(s)==currentSystems.end())
		 {
			 s->onEntityAdd(entity);
		 }
		 else if(s!=nullptr&&currentSystems.find(s)!=currentSystems.end()&&!s->matchesRequirements(entity))
		 {
			 s->onEntityRemove(entity);
		 }
	 }
}
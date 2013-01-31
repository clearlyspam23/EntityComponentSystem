#include "Entity.h"
#include "EntityManager.h"
#include "Component.h"
#include <algorithm>

using namespace std;

Entity::~Entity()
{
	for(auto i = _componentsMap.begin(); i != _componentsMap.end(); ++i)
	{
		delete i->second;
	}
}

bool operator==(const Entity& entity1, const Entity& entity2)
{
	return entity1.getId()==entity2.getId();
}

bool operator!=(const Entity& entity1, const Entity& entity2)
{
	return entity1.getId()!=entity2.getId();
}

Component* Entity::getComponent(size_t id)
{
	return _componentsMap.at(id);
}

void Entity::addComponent(Component* component)
{
	_componentsMap.insert(pair<size_t, Component*>(component->componentId(), component));
	_components.set(component->componentId());
}

void Entity::removeComponent(Component* component)
{
	auto it = _componentsMap.find(component->componentId());
	delete it->second;
	_componentsMap.erase(it);
	_components.reset(component->componentId());
}

void Entity::removeComponent(size_t id)
{
	auto it = _componentsMap.find(id);
	delete it->second;
	_componentsMap.erase(it);
	_components.reset(id);
}
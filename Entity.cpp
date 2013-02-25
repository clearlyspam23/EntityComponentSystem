#include "Entity.h"
#include "EntityManager.h"
#include "Component.h"
#include <algorithm>

using namespace std;

Entity::~Entity()
{
	for(auto i = _componentsMap.begin(); i != _componentsMap.end(); ++i)
	{
		BaseComponent::destroy(i->second, i->first);
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

BaseComponent* Entity::getComponent(size_t id)
{
	return _componentsMap.at(id);
}

void Entity::addComponent(size_t id)
{
	_componentsMap.insert(pair<size_t, BaseComponent*>(id, BaseComponent::create(id)));
	_components.set(id);
}

void Entity::removeComponent(size_t id)
{
	auto it = _componentsMap.find(id);
	BaseComponent::destroy(it->second, it->first);
	_componentsMap.erase(it);
	_components.reset(id);
}
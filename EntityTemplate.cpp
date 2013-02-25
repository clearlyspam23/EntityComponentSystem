#include "EntityTemplate.h"
#include "Entity.h"
#include "Component.h"

void EntityTemplate::requireComponent(const size_t& id)
{
	_components.set(id);
}

std::shared_ptr<Entity>& EntityTemplate::constructEntity(std::shared_ptr<Entity>& entity) const
{
	for(size_t i = 0; i < _components.size(); ++i)
	{
		if(_components.test(i))
			entity->addComponent(i);
	}
	return entity;
}

std::shared_ptr<Entity>& EntityTemplate::reconstructEntity(std::shared_ptr<Entity>& entity) const
{
	COMPONENT_ID ecomponents = entity->getIncludedComponents();
	for(size_t i = 0; i < _components.size(); ++i)
	{
		if(ecomponents.test(i)&&!_components.test(i))
			entity->removeComponent(i);
		else if(_components.test(i)&&!ecomponents.test(i))
			entity->addComponent(i);
	}
	return entity;
}
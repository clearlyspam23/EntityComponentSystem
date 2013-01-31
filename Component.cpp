#include "Component.h"

Component* component::create(const size_t& id)
{
	ComponentRegistry& registry = getComponentRegistry();
	return registry[id]();
}

void component::destroy(Component* component)
{
	delete component;
}
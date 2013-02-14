#include "Component.h"

Component* component::create(size_t id)
{
	ComponentRegistry& registry = getComponentRegistry();
	return registry[id]();
}

void component::destroy(Component* component)
{
	delete component;
}
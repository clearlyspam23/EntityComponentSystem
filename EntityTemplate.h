#ifndef ENTITYTEMPLATE_H
#define ENTITYTEMPLATE_H

#include "Typedefs.h"
#include <bitset>
#include <string>
#include <memory>

class Entity;

class EntityTemplate
{
public:
	friend class EntityManager;
	void requireComponent(const size_t& id);
	template <typename T>
	void requireComponent() { _components.set(T::COMPONENTID()); }
protected:
	std::shared_ptr<Entity>& constructEntity(std::shared_ptr<Entity>& entity) const;
	std::shared_ptr<Entity>& reconstructEntity(std::shared_ptr<Entity>& entity) const;
private:
	COMPONENT_ID _components;
};

#endif //ENTITYTEMPLATE_H
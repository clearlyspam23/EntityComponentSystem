#ifndef ENTITY_H
#define ENTITY_H

#include "Typedefs.h"
#include <map>
#include <bitset>

class EntityManager;

class Component;

class Entity
{
public:
	friend class EntityManager;
	~Entity();

	template <typename T>
	T* getComponent() const;
	Component* getComponent(size_t id);

	template <typename T>
	T* addComponent();
	void addComponent(Component* component);

	template <typename T>
	void removeComponent();
	void removeComponent(Component* component);
	void removeComponent(size_t id);

	template <typename T>
	bool hasComponent() const;

	const COMPONENT_ID& getIncludedComponents() const { return _components; }

	const ENTITY_ID& getId() const { return _id; }
private:
	COMPONENT_ID _components;
	ENTITY_ID _id;
	std::map<size_t, Component*> _componentsMap;

	Entity() : _id(), _components(0), _componentsMap() {}
	Entity(ENTITY_ID id) : _id(id), _components(0), _componentsMap() {}

	void setId(const ENTITY_ID id) { _id = id; }

	Entity(const Entity& entity);
	Entity& operator=(Entity entity);

};

bool operator==(const Entity& entity1, const Entity& entity2);
bool operator!=(const Entity& entity1, const Entity& entity2);

template <typename T>
T* Entity::getComponent() const
{
	return (T*) _componentsMap.at(T::COMPONENTID());
}

template <typename T>
T* Entity::addComponent()
{
	//what is this shit I don't even...
	T* t = new T();
	_componentsMap.insert(std::pair<COMPONENT_ID, Component*>(T::COMPONENTID(), t));
	_components.set(T::COMPONENTID());
	return t;
}

template <typename T>
void Entity::removeComponent()
{
	auto it = _componentsMap.find(T::COMPONENTID());
	delete it->second;
	_componentsMap.erase(it);
	_components.reset(T::COMPONENTID());
}

template <typename T>
bool Entity::hasComponent() const
{
	return _components.test(T::COMPONENTID());
}

#endif //ENTITY_H
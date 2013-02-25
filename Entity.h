#ifndef ENTITY_H
#define ENTITY_H

#include "Typedefs.h"
#include <map>
#include <bitset>

class EntityManager;
class EntityTemplate;
class BaseComponent;

class Entity
{
public:
	friend class EntityManager;
	friend class EntityTemplate;
	~Entity();

	template <typename T>
	T* getComponent() const;
	BaseComponent* getComponent(size_t id);

	template <typename T>
	bool hasComponent() const;

	const COMPONENT_ID& getIncludedComponents() const { return _components; }

	const ENTITY_ID& getId() const { return _id; }
private:
	COMPONENT_ID _components;
	ENTITY_ID _id;
	std::map<size_t, BaseComponent*> _componentsMap;

	Entity() : _id(), _components(0), _componentsMap() {}
	Entity(ENTITY_ID id) : _id(id), _components(0), _componentsMap() {}

	void setId(const ENTITY_ID id) { _id = id; }

	Entity(const Entity& entity);
	Entity& operator=(Entity entity);

	template <typename T>
	T* addComponent();
	void addComponent(size_t id);

	template <typename T>
	void removeComponent();
	void removeComponent(size_t id);

};

bool operator==(const Entity& entity1, const Entity& entity2);
bool operator!=(const Entity& entity1, const Entity& entity2);

template <typename T>
T* Entity::getComponent() const
{
	return static_cast<T*>(_componentsMap.at(Component<T>::COMPONENTID()));
}

template <typename T>
T* Entity::addComponent()
{
	//what is this shit I don't even...
	T* t = new T();
	_componentsMap.insert(std::pair<COMPONENT_ID, BaseComponent*>(Component<T>::COMPONENTID(), t));
	_components.set(Component<T>::COMPONENTID());
	return t;
}

template <typename T>
void Entity::removeComponent()
{
	auto it = _componentsMap.find(Component<T>::COMPONENTID());
	delete it->second;
	_componentsMap.erase(it);
	_components.reset(Component<T>::COMPONENTID());
}

template <typename T>
bool Entity::hasComponent() const
{
	return _components.test(Component<T>::COMPONENTID());
}

#endif //ENTITY_H
#ifndef COMPONENT_H
#define COMPONENT_H

#include "Typedefs.h"
#include <vector>
#include <utility>

#define REGISTER_COMPONENT(type, id)                     \
size_t Component<type>::ID = id;                         \
Component<type>::ComponentRegistration Component<type>::reg(id)

class BaseComponent;

typedef BaseComponent* (*CreateComponentFunc)();
typedef void (*DestroyComponentFunc)(BaseComponent*);
typedef std::vector<std::pair<CreateComponentFunc, DestroyComponentFunc>> ComponentRegistry;

class BaseComponent
{
public:
	static BaseComponent* create(size_t id) { return registry()[id].first(); }
	static void destroy(BaseComponent* component, size_t id) { registry()[id].second(component); }
protected:
	BaseComponent() {}
	~BaseComponent() {}
	static ComponentRegistry& registry() { static ComponentRegistry reg(MAX_COMPONENTS); return reg; }
};


template <typename T>
class Component : public BaseComponent
{
private:
	static BaseComponent* create() { return new T; }
	static void destroy(BaseComponent* t) { delete static_cast<T*>(t); }
	class ComponentRegistration
	{
	public:
		ComponentRegistration(size_t id){ BaseComponent::registry()[id] = std::pair<CreateComponentFunc, DestroyComponentFunc>(create, destroy); }
	};
public:
	size_t componentId() { return ID; }
	static size_t COMPONENTID() { return ID; }
protected:
	Component() {}
	~Component() {}
private:
	static size_t ID;
	static ComponentRegistration reg;
};


#endif //COMPONENT_H
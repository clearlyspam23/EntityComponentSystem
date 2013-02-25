#ifndef SYSTEM_H
#define SYSTEM_H
#include <map>
#include <memory>
#include "Entity.h"

#define REGISTER_SYSTEM(type, id)                                             \
    unsigned int System<type>::systemID = id;

class BaseSystem
{
public:
	virtual ~BaseSystem() {}
	const COMPONENT_ID& requirements() { return _requirements; }
	bool matchesRequirements(const std::shared_ptr<Entity>& entity) const { return (entity->getIncludedComponents()&_requirements)==_requirements; }
	virtual unsigned int systemId() = 0;
	virtual void onTick(const double& delta, EntityManager& manager) = 0;
	virtual void onEntityAdd(std::shared_ptr<Entity>& e) {}
	virtual void onEntityRemove(std::shared_ptr<Entity>& e) {}
	virtual void onSystemAdd(EntityManager& manager) {}
	template <typename U>
	void require() { _requirements.set(Component<U>::COMPONENTID()); }
protected:
	BaseSystem() : _requirements(0) {}
private:
	COMPONENT_ID _requirements;
};

template <typename T>
class System : public BaseSystem
{
public:
	virtual unsigned int systemId() { return systemID; }
	static unsigned int SYSTEMID() { return systemID; }
private:
	static unsigned int systemID;
};

#endif //SYSTEM_H
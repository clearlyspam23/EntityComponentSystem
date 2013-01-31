#ifndef SYSTEM_H
#define SYSTEM_H
#include <set>
#include <memory>
#include "Entity.h"

class System
{
public:
	friend class EntityManager;
	System() : _requirements(0) {}
	virtual ~System() {}
	virtual void onTick(const double& delta, EntityManager& manager) = 0;
	virtual void onEntityAdd(const std::shared_ptr<Entity>& e) = 0;
	virtual void onEntityRemove(const std::shared_ptr<Entity>& e) = 0;
	const COMPONENT_ID& requirements() { return _requirements; }
	template <typename T>
	void require() { _requirements.set(T::COMPONENTID()); }
	bool matchesRequirements(const std::shared_ptr<Entity>& entity) const { return (entity->getIncludedComponents()&_requirements)==_requirements; }
private:
	COMPONENT_ID _requirements;
};

#endif //SYSTEM_H
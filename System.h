#ifndef SYSTEM_H
#define SYSTEM_H
#include <map>
#include <memory>
#include "Entity.h"

#define SYSTEM                                                                \
private: static unsigned int systemID;                                        \
public: static const unsigned int& SYSTEMID() { return systemID; }            \
		virtual const unsigned int& systemId() { return systemID; }   

#define REGISTER_SYSTEM(type, id)                                             \
    unsigned int type::systemID = id;

class System
{
public:
	System() : _requirements(0) {}
	virtual ~System() {}
	virtual void onTick(const double& delta, EntityManager& manager) = 0;
	virtual void onEntityAdd(const std::shared_ptr<Entity>& e) {};
	virtual void onEntityRemove(const std::shared_ptr<Entity>& e) {};
	virtual const unsigned int& systemId() = 0;
	const COMPONENT_ID& requirements() { return _requirements; }
	template <typename T>
	void require() { _requirements.set(T::COMPONENTID()); }
	bool matchesRequirements(const std::shared_ptr<Entity>& entity) const { return (entity->getIncludedComponents()&_requirements)==_requirements; }
private:
	COMPONENT_ID _requirements;
};

#endif //SYSTEM_H
#ifndef COMPONENT_H
#define COMPONENT_H

#include "Typedefs.h"
#include "ComponentImpl.h"

#define COMPONENT                                                             \
private: static size_t componentID;                                           \
public: static const size_t& COMPONENTID() { return componentID; }            \
		virtual const size_t& componentId() { return componentID; }                      

#define REGISTER_COMPONENT(type, id)                                          \
    size_t type::componentID = id;                                            \
    namespace component {                                                     \
    namespace                                                                 \
    {                                                                         \
        template<class T>                                                     \
        class ComponentRegistration;                                          \
                                                                              \
        template<>                                                            \
        class ComponentRegistration<type>                                     \
        {                                                                     \
            static const ::component::RegistryEntry<type>& reg;               \
        };                                                                    \
                                                                              \
        const ::component::RegistryEntry<type>&                               \
            ComponentRegistration<type>::reg =                                \
                ::component::RegistryEntry<type>::Instance(id);               \
	}}

namespace component{
	Component* create(const size_t& id);
	void destroy(Component* component);
}


class Component
{
public:
	virtual ~Component() {}
	virtual const size_t& componentId() = 0;
};

#endif //COMPONENT_H
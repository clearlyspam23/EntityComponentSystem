#ifndef COMPONENT_H
#define COMPONENT_H

#include "Typedefs.h"
#include <vector>
#include <string>

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


class Component
{
public:
	virtual ~Component() {}
	virtual const size_t& componentId() = 0;
};

namespace component{

	Component* create(size_t id);
	void destroy(Component* component);



	typedef Component* (*CreateComponentFunc)();
	typedef std::vector<CreateComponentFunc> ComponentRegistry;

	inline ComponentRegistry& getComponentRegistry()
	{
		static ComponentRegistry reg(MAX_COMPONENTS);
		return reg;
	}

	template <typename T>
	Component* createComponent(){
		return new T;
	}

	template<typename T>
	class RegistryEntry
	{
		public:
            static RegistryEntry<T>& Instance(size_t id)
            {
                static RegistryEntry<T> inst(id);
                return inst;
            }
          private:
            RegistryEntry(size_t id)
            {
                CreateComponentFunc func = createComponent<T>;
				ComponentRegistry& reg = getComponentRegistry();
				reg[id] = func;
            }
            RegistryEntry(const RegistryEntry<T>&);
            RegistryEntry& operator=(const RegistryEntry<T>&);
	};
} //component

#endif //COMPONENT_H
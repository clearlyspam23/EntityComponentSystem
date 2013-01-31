#include "Typedefs.h"
#include <vector>
#include <string>

class Component;

namespace component{

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
            static RegistryEntry<T>& Instance(const size_t& id)
            {
                static RegistryEntry<T> inst(id);
                return inst;
            }
          private:
            RegistryEntry(const size_t& id, const std::string& name)
            {
                CreateComponentFunc func = createComponent<T>;
				ComponentRegistry& reg = getComponentRegistry();
				reg[id] = func;
            }
            RegistryEntry(const RegistryEntry<T>&);
            RegistryEntry& operator=(const RegistryEntry<T>&);
	};
} //component
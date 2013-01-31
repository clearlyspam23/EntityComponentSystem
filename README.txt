my implementation of the component based entity system

To use this code, download and compile it on your choice of compiler.

*******COMPONENTS*******

to define a component, first publicly extend the Component class (defined in Component.h)
then, at the top of your class declaration, include the macro COMPONENT
then, at some point in your code (preferably in a .cpp), include the macro REGISTER_COMPONENT(class, number)
where class is the typename of the Component class you wish to register, and number is the id number you wish to give it.
please note that 0<=id<MAX_COMPONENTS (defined in typedefs.h, default 32)

for example:

ComponentCoordinate2d.h

	#ifndef COMPONENT_COORDINATE_2D_H
	#define COMPONENT_COORDINATE_2D_H

	#include <Component.h>

	class ComponentCoordinate2d : public Component
	{
		COMPONENT
	public:
		double _x;
		double _y;
	};

	#endif //COMPONENT_COORDINATE_2D_H

ComponentCoordinate2d.cpp

	REGISTER_COMPONENT(ComponentCoordinate2d, 0);

*******SYSTEMS********

to define a System, simply publicly extend System, and overwrite the method

	virtual void onTick(const double& delta, EntityManager& manager);

from here, you can query the EntityManager for any Entities it may have
for convenience, you may also utilize the methods:

	template <typename T>
	void require();

	bool matchesRequirements(const std::shared_ptr<Entity>& entity);

require<typename T>() marks a component as required by this System

matchesRequirements(const std::shared_ptr<Entity>& entity) returns true if the entity passed to it possesses all of the required components

you may also want to overwrite the methods:

	virtual void onEntityAdd(const std::shared_ptr<Entity>& e);
	virtual void onEntityRemove(const std::shared_ptr<Entity>& e);

which gets called by the EntityManager when an Entity matching this system's requirements are added to or removed from the corresponding EntityManager

*******Entity, EntityManager, EntityTemplate*******

Entities, at their very basic, are IDs with components attached to them.
the lifecycle of Entities is controlled entirely by a corresponding EntityManager
to create an Entity, first create an EntityManager, then call it's createEntity method
Example:
	EntityManager m;
	std::shared_ptr<Entity> e = m.createEntity();

this will create a blank entity with no attached components

The attaching of components is handled entirely by EntityTemplates. EntityTemplates define a collection of Components which define an entity type.

to make an EntityTemplate require a component, use

	void requireComponent(const size_t& id);

	or

	template <typename T>
	void requireComponent();

Then, in order to construct an Entity matching this EntityTemplate's definition, construct it using:

	std::shared_ptr<Entity> createEntity(const EntityTemplate& t);

or modify an existing entity by using

	void modifyEntity(std::shared_ptr<Entity> e, const EntityTemplate& t);

which causes an already existing entity's components to change to match the new EntityTemplate provided
any components which the two templates have in common is saved, and so any state it holds will remain after modification

To delete entities, call:

	void removeEntity(std::shared_ptr<Entity> entity);

And in order to get Entities, call:

	const std::set<std::shared_ptr<Entity>>& getEntities() const;

	std::set<std::shared_ptr<Entity>> getEntitiesWithComponents(const COMPONENT_ID& components) const;

in order for Entity's to be operated on, Systems have to be added to the EntityManager, to do this, call:

	void addSystem(System* system);

please note that, on adding a system to an EntityManager, the manager assumes control of the lifecycle of the System, 
and will destroy the System when it dies

then, in your game loop, make sure you call:

	void onTick(float delta);

so for instance, example code may look like
	
	EntityTemplate t;
	EntityTemplate s;
	t.require<ComponentCoordinate>();
	t.require<ComponentVelocity>();
	s.require<ComponentDrawable>();
	EntityManager m;
	m.addSystem(new SystemMovement());
	m.addSystem(new SystemDraw());
	std::shared_ptr<Entity> e = m.constructEntity(t);
	m.constructEntity(s);
	m.modifyEntity(e, s);
	for(;;)
	{
		m.onTick();
	}



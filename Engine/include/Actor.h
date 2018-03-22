#pragma once
#include <vector>
#include "Typedefs.h"
#include "Transform.h"

class Component;
class Renderable;

class Actor {

public:
	Transform transform;

	Actor();
	~Actor();
	
	void start();
	void update(float deltaTime) const;

	template <typename ComponentType>
	ComponentType* addComponent();

	template <typename ComponentType>
	ComponentType* getComponentOfType() const;

	template <typename ComponentType>
	std::vector<ComponentType*> getComponentsOfType() const;

	void setRenderable(Renderable* renderable);
	Renderable* getRenderable() const;

private:
	std::vector<Component*> mComponents;
	Renderable* mRenderable;	

	Component* addComponent(Component* component);
};

template <typename ComponentType>
ComponentType* Actor::addComponent()
{
	Component* component = new ComponentType();
	addComponent(component);
	return component;
}

template <typename ComponentType>
ComponentType* Actor::getComponentOfType() const
{
	for (auto component : mComponents)
	{
		bool isComponentOfTypeLookingFor = dynamic_cast<ComponentType*>(component) != nullptr;
		if (isComponentOfTypeLookingFor)
		{
			return component;
		}
	}

	return nullptr;
}

template <typename ComponentType>
std::vector<ComponentType*> Actor::getComponentsOfType() const
{
	std::vector<ComponentType*> componentsOfType;
	for (auto component : mComponents)
	{
		bool isComponentOfTypeLookingFor = dynamic_cast<ComponentType*>(component) != nullptr;
		if (isComponentOfTypeLookingFor)
		{
			componentsOfType.push_back(component);
		}
	}

	return componentsOfType;
}
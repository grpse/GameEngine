#pragma once
#include <vector>
#include "Transform.h"
#include "Renderable.h"
#include "Component.h"
#include "Typedefs.h"

class Actor {

public:
	Transform transform;

	void start();
	void update(float deltaTime) const;

	template <typename ComponentType>
	ComponentType* addComponent();

	template <typename ComponentType>
	ComponentType* getComponentOfType() const;

	template <typename ComponentType>
	std::vector<ComponentType*> getComponentsOfType() const;

	template <typename RenderableType>
	void setRenderable();

	template <typename RenderableType>
	RenderableType* getRenderable() const;

	Renderable* getRenderable() const;

private:
	std::vector<Component*> mComponents;
	Renderable* mRenderable;
	
	Component* addComponent(Component* component);
	void setRenderable(Renderable* renderable);
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

template <typename RenderableType>
void Actor::setRenderable()
{
	if (mRenderable != nullptr)
	{
		delete mRenderable;
	}

	setRenderable(new RenderableType());
}

template <typename RenderableType>
RenderableType* Actor::getRenderable() const
{
	return dynamic_cast<RenderableType*>(mRenderable);
}
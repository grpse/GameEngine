#include "Actor.h"
#include "Component.h"
#include "Renderable.h"
#include "Transform.h"

Actor::Actor()
{
	mTransform = new Transform;
}

Actor::~Actor()
{
	delete mTransform;
}

void Actor::start()
{
	for (Component* component : mComponents)
	{
		component->start();
	}
}

void Actor::update(float deltaTime) const
{
	for (Component* component : mComponents)
	{
		component->update(deltaTime, *this);
	}
}

Component* Actor::addComponent(Component * component)
{
	mComponents.push_back(component);
	return component;
}

void Actor::setRenderable(Renderable * renderable)
{
	mRenderable = renderable;
}

Renderable* Actor::getRenderable() const
{
	return mRenderable;
}

Transform& Actor::getTransform() const
{
	return *mTransform;
}
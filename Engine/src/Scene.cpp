#include <algorithm>
#include "Scene.h"
#include "Material.h"
#include "FrameBuffer.h"
#include "ShadowRenderer.h"
#include "Window.h"
#include "Renderable.h"
#include "Actor.h"
#include "Camera.h"
#include "Light.h"
#include "Renderer.h"
#include "ServiceLocator.h"

void Scene::setup()
{
	mRenderer = new Renderer;
}

void Scene::addChild(const Actor& actor)
{
	mActors.push_back((Actor*)&actor);
}

void Scene::addChild(const Camera& camera)
{
	mCameras.push_back((Camera*)&camera);
}

void Scene::addChild(const Light& light)
{
	mLights.push_back((Light*)&light);
	Material::setLightsVectorNames((uint)mLights.size());
}

void Scene::updateComponents(float deltaTime)
{
	update(deltaTime);
	typedef std::pair<Renderable*, const Actor*> RenderableActor;

	const uint RenderablesQueueTypesCount = (uint)Renderable::QueueType::Count;

	std::vector<RenderableActor> renderQueues[RenderablesQueueTypesCount];

	for (const Actor* actor : mActors)
	{
		if (actor->getRenderable() != nullptr)
		{
			uint QueueTypeIndex = (uint)actor->getRenderable()->getRenderQueue();
			renderQueues[QueueTypeIndex].push_back({ actor->getRenderable(), actor });
		}

		actor->update(deltaTime);
	}

	// TODO: Remove this fixed step from here
	// Clear shadow map framebuffer
	Locator::locateShadowRenderer()->getFrameBuffer().bind();
	mRenderer->clearColorAndDepth();
	Locator::locateShadowRenderer()->getFrameBuffer().unbind();

	// Reset viewport
	mRenderer->clearColorAndDepth();

	Rect ScreenRect = Locator::locateWindow()->getViewport();
	mRenderer->setViewport(ScreenRect);
	// render on queue type order [Opaque, Cutoff, Transparent]
	std::vector<RenderableActor>& opaqueRenderables = renderQueues[(uint)Renderable::QueueType::Opaque];

	for (const Camera* camera : mCameras)
	{
		/*
		for (RenderableActor renderActor : renderQueues[i])
		{
		mRenderer->setViewport(ScreenRect);
		renderActor.first->preRender(*camera, mLights[0], mLights.size(), *renderActor.second, *mRenderer);
		}
		*/

		for (RenderableActor renderActor : opaqueRenderables)
		{
			renderActor.first->render(camera[0], mLights[0], mLights.size(), renderActor.second[0], mRenderer[0]);
		}

		/*
		for (RenderableActor renderActor : renderQueues[i])
		{
		mRenderer->setViewport(ScreenRect);
		renderActor.first->postRender(*camera, mLights[0], mLights.size(), *renderActor.second, *mRenderer);
		}
		*/
	}

}
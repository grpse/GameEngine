#include <algorithm>
#include "Scene.h"
#include "Material.h"
#include "FrameBuffer.h"
#include "ShadowRenderer.h"
#include "Window.h"

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

	std::vector<RenderableActor> preRenderQueues[RenderablesQueueTypesCount];
	std::vector<RenderableActor> renderQueues[RenderablesQueueTypesCount];
	std::vector<RenderableActor> postRenderQueues[RenderablesQueueTypesCount];

	for (const Actor* actor : mActors)
	{
		if (actor->getRenderable() != nullptr)
		{
			uint QueueTypeIndex = (uint)actor->getRenderable()->getRenderQueue();
			renderQueues[QueueTypeIndex].push_back({ actor->getRenderable(), actor });

			if (actor->getRenderable()->hasPrePassStep())
				preRenderQueues[QueueTypeIndex].push_back({ actor->getRenderable(), actor });

			if (actor->getRenderable()->hasPostPassStep())
				postRenderQueues[QueueTypeIndex].push_back({ actor->getRenderable(), actor });
		}

		actor->update(deltaTime);
	}

	// TODO: Remove this fixed step from here
	// Clear shadow map framebuffer
	ShadowRenderer::getFrameBuffer().bind();
	mRenderer.clearColorAndDepth();
	ShadowRenderer::getFrameBuffer().unbind();

	// Reset viewport
	mRenderer.clearColorAndDepth();

	Rect ScreenRect = Window::getInstance().getViewport();

	// render on queue type order [Opaque, Cutoff, Transparent]
	for (uint i = 0; i < RenderablesQueueTypesCount; i++)
	{
		for (const Camera* camera : mCameras)
		{
			for (RenderableActor renderActor : preRenderQueues[i])
			{
				mRenderer.setViewport(ScreenRect);
				renderActor.first->preRender(*camera, mLights[0], mLights.size(), *renderActor.second, mRenderer);
			}

			for (RenderableActor renderActor : renderQueues[i])
			{
				mRenderer.setViewport(ScreenRect);
				renderActor.first->render(*camera, mLights[0], mLights.size(), *renderActor.second, mRenderer);
			}

			for (RenderableActor renderActor : postRenderQueues[i])
			{
				mRenderer.setViewport(ScreenRect);
				renderActor.first->postRender(*camera, mLights[0], mLights.size(), *renderActor.second, mRenderer);
			}
		}
	}
}
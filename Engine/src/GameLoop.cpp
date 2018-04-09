#include <GL/glew.h>
#include <iostream>
#include "LinearMath.h"
#include "GameLoop.h"
#include "Time.h"
#include "ParticleSystem.h"
#include "Camera.h"
#include "GLErrorHandling.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Loader.h"
#include <SOIL.h>
#include "Mesh.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Renderer.h"
#include "ParticleRenderer.h"
#include "FrameBuffer.h"
#include "BillboardRenderer.h"
#include "ShadowRenderer.h"
#include "Input.h"
#include "Light.h"
#include "SkyboxRenderer.h"

double deltaTimeInSecondsFraction = 0;
double mElapsedSecond = 0;

Window mWindow;
Camera mCamera;
Matrix4 mProjectionMatrix;



static Vector3 quadVertices[] = {
	Vector3(-0.5, -0.5, 0),
	Vector3( 0.5, -0.5, 0),
	Vector3( 0.5, 0.5, 0),
	Vector3(-0.5, 0.5, 0),
};

void GameLoop::start()
{

	Rect viewport = { 0, 0, 800, 600 };
	mWindow.setViewport(viewport);
	mWindow.start();
	
	Camera::Format cameraFormat;
	cameraFormat.fieldOfView = 45.0f;
	cameraFormat.aspectRatio = viewport.width / viewport.height;
	cameraFormat.nearPlane = 0.001f;
	cameraFormat.farPlane = 10000.0;

	mCamera.setFormat(cameraFormat, Camera::Type::Perspective);
	mCamera.transform.setLocalPosition({ 0, 2, -10 });
	mCamera.transform.setLocalRotation(Vector3(3.1415f/2.0f, 3.1415f, 0));
		 
	Vector3 position = mCamera.transform.getLocalPosition();
	Vector3 rotation = Math::eulerAngles(mCamera.transform.getLocalRotation()) * 3.14159f / 180.f;

	auto updateCameraPositionAndRotation = [&]() {

		float moveDiff = (float)(40.0 * Time::getDeltaTime());

		Vector3 front = mCamera.transform.getFront();
		Vector3 right = mCamera.transform.getRight();
		if (Input::isPressedKey(W)) {
			position += Math::normalize(front) * moveDiff;
		}

		if (Input::isPressedKey(S)) {
			position -= Math::normalize(front) * moveDiff;
		}

		if (Input::isPressedKey(D)) {
			position += Math::normalize(right) * moveDiff;
		}

		if (Input::isPressedKey(A)) {
			position -= Math::normalize(right) * moveDiff;
		}

		mCamera.transform.setLocalRotation(Quaternion(rotation));
		mCamera.transform.setLocalPosition(position);


		// UPDATE FRONT AND RIGHT VECTORS
		mCamera.transform.getWorldMatrix();
	};

	auto updateCameraOrientation = [&]() {
		double x, y;
		Input::getMouseDelta(x, y);

		float rotationDiff = (float)(5.0 * Time::getDeltaTime());

		if (x > 0 || x < 0) {
			rotation.y += x * rotationDiff;
		}

		if (y > 0 || y < 0) {
			const float halfPi = Math::radians(90.0);
			float nextRotationOnX = rotation.x - y * rotationDiff;
			rotation.x = Math::clamp(nextRotationOnX, -halfPi, halfPi);
		}

		mCamera.transform.setLocalRotation(Quaternion(rotation));
		mCamera.transform.getWorldMatrix();
	};
	
	//ParticleRenderer particleRenderer;
	MeshRenderer meshRenderer;

	//Mesh suzanne = Loader::loadMeshAsArray("cow_up.in");
	Mesh suzanne = Loader::loadSimpleMesh("suzanne.obj");
	Transform suzanneTransform;

	Mesh quad = Mesh::createQuad();
	Transform quadTransform;
	quadTransform.setLocalPosition({ 0, -2, 0 });
	quadTransform.setLocalScale({ 5, 5, 5 });
	quadTransform.setLocalRotation(Vector3(Math::radians(180.0), 0, 0));

	suzanneTransform.setLocalPosition({ -0.5, 0.1, -1 });
	// suzanneTransform.setLocalScale({ 0.001, 0.001, 0.001 });
	suzanneTransform.setLocalScale({1, 1, 1 });
	suzanneTransform.setLocalRotation(Vector3(0, 0, 0));

	Renderer renderer;
	renderer.setFrontCounterClockwise();
	
	BillboardRenderer billboardRenderer;
	ShadowRenderer shadowRenderer;


	Light directional;
	directional.direction = Vector3(-1, -1, -1);
	directional.color = Color32(1, 1, 1, 1);
	directional.intensity = 1;
	
	float angle = 0;
	float mx = 0;
	float mz = 0;

	// load skybox
	SkyboxRenderer skyboxRenderer;
	skyboxRenderer.setCubeMap(Loader::loadCubeMap("skybox/cloudysky", "png"));
	
	Time::startDeltaTime();

	while (mWindow.isOpen()) {
		
		/*
		float dt = (float)Time::getDeltaTime();
		angle += dt * 1;
		mx = cos(angle);
		mz = sin(angle);

		suzanneTransform.setLocalPosition({ mx, 0, mz });
		*/
		
		//mParticleSystem.emitParticle(Vector3(0, -5, 0));
		// mParticleSystem.update();

		//shadowRenderer.getShadowBuffer().setLayout({ 1024, 1024, 0 });
		//shadowRenderer.getShadowBuffer().bind();
		//
		//renderer.clearColorAndDepth();
		//// TODO: in order to draw shadow map, we need a shadow renderer
		////			where geometries, lights and camera will create the
		////			depth buffer appropriated to use on shadow projection.
		//shadowRenderer.renderShadowMap(mCamera, suzanne, suzanneTransform, directional, renderer);
		//shadowRenderer.renderShadowMap(mCamera, quad, quadTransform, directional, renderer);
		//
		//shadowRenderer.getFrameBuffer().unbind();

		renderer.clearColorAndDepth();
		renderer.setViewport(viewport);

		renderer.setRenderMode(Renderer::Mode::Triangles);
		skyboxRenderer.render(mCamera, renderer);

		meshRenderer.render(mCamera, quad, quadTransform, directional, renderer);
		meshRenderer.render(mCamera, suzanne, suzanneTransform, directional, renderer);

		//shadowRenderer.renderAdditiveShadow(mCamera, suzanne, suzanneTransform, directional, renderer);
		//shadowRenderer.renderAdditiveShadow(mCamera, quad, quadTransform, directional, renderer);
		//meshRenderer.render(mCamera, suzanne, suzanneTransform, directional, shadowRenderer.getShadowMap(), renderer);
		//meshRenderer.render(mCamera, quad, quadTransform, directional, shadowRenderer.getShadowMap(), renderer);
		
		//particleRenderer.render(mParticleSystem, mCamera, renderer);


		if (shadowRenderer.getShadowBuffer().isComplete())
		{
			renderer.setRenderMode(Renderer::Mode::Quads);
			billboardRenderer.render(shadowRenderer.getShadowMap(), { 0.5f, -0.5f, 0.5f, 0.5f }, renderer);
		}
		//billboardRenderer.render(particleTexture, { 0.5f, -0.5f, 0.5f, 0.5f }, renderer);

		
		mWindow.swapBuffers();
		mWindow.pollEvents();
		Time::updateDeltaTime();
		updateCameraPositionAndRotation();
		updateCameraOrientation();		
	}

	//TwTerminate();
	mWindow.finish();
}
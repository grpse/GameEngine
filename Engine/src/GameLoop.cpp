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
#include "MeshSoftwareRenderer.h"
#include "Transform.h"
#include "Renderer.h"
//#include "ParticleRenderer.h"
#include "FrameBuffer.h"
//#include "BillboardRenderer.h"
#include "ShadowRenderer.h"
#include "Input.h"
#include "Light.h"
#include "SkyboxRenderer.h"
#include "ServiceLocator.h"
#include <AntTweakBar.h>

bool shouldReset = false;

static Vector3 quadVertices[] = {
	Vector3(-0.5, -0.5, 0),
	Vector3( 0.5, -0.5, 0),
	Vector3( 0.5, 0.5, 0),
	Vector3(-0.5, 0.5, 0),
};

void GameLoop::start()
{
	Window mWindow;
	Camera mCamera;
	Matrix4 mProjectionMatrix;

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
	Vector3 originalPosition = position;
	Vector3 originalRotation = rotation;

	auto updateCameraPositionAndRotation = [&]() {

		float moveDiff = (float)(40.0 * Time::getDeltaTime());

		Vector3 front = mCamera.transform.getFront();
		Vector3 right = mCamera.transform.getRight();
		if (Input::isPressedKeyRepeated(W)) {
			position += Math::normalize(front) * moveDiff;
		}

		if (Input::isPressedKeyRepeated(S)) {
			position -= Math::normalize(front) * moveDiff;
		}

		if (Input::isPressedKeyRepeated(D)) {
			position += Math::normalize(right) * moveDiff;
		}

		if (Input::isPressedKeyRepeated(A)) {
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
			const float halfPi = Math::pi<float>() / 2.0f;
			float nextRotationOnX = rotation.x - y * rotationDiff;
			rotation.x = Math::clamp(nextRotationOnX, -halfPi, halfPi);
		}

		mCamera.transform.setLocalRotation(Quaternion(rotation));
		mCamera.transform.getWorldMatrix();
	};
	
	auto resetButtom = [&]() {
		if (Input::wasPressedKey(R))
		{
			position = originalPosition;
			rotation = originalRotation;
			mCamera.transform.setLocalPosition(position);
			mCamera.transform.setLocalRotation(rotation);
			mCamera.transform.getWorldMatrix();
		}
	};

	//ParticleRenderer particleRenderer;

	Mesh suzanne = Loader::loadMeshAsArrayForDynamic("cow_up.in", 0.001f, true);
	//Mesh suzanne = Loader::loadSimpleMesh("suzanne.obj");
	Transform suzanneTransform;

	Mesh quad = Mesh::createQuad();
	Transform quadTransform;
	quadTransform.setLocalPosition({ 0, -2, 0 });
	quadTransform.setLocalScale({ 5, 5, 5 });
	quadTransform.setLocalRotation(Vector3(Math::radians(180.0), 0, 0));

	suzanneTransform.setLocalPosition({ 0, 0, 0 });
	suzanneTransform.setLocalScale({1, 1, 1 });
	suzanneTransform.setLocalRotation(Vector3(0, 0, 0));

	double xAngle = 0;
	double yAngle = 0;
	float distanceFromCow = 1.0f;

	auto updateDistanceFromCow = [&]() {
		double scrollX, scrollY;
		Input::getScrollOffsetDelta(scrollX, scrollY);
		if (scrollY > 0 || scrollY < 0) {
			distanceFromCow += scrollY * 10 * Time::getDeltaTime();
			distanceFromCow = Math::max(0.01f, distanceFromCow);
		}

		mCamera.transform.getWorldMatrix();
	};

	auto updateCameraOrientationAndPositionLookAtCow = [&]() {
		double x, y;
		Input::getMouseDelta(x, y);

		float rotationDiff = (float)(5.0 * Time::getDeltaTime());

		if (x > 0 || x < 0) {
			xAngle += Math::radians(x);
			
			//xAngle += Math::radians(x);
			position.x = Math::sin(xAngle) * distanceFromCow;
			position.z = -Math::cos(xAngle) * distanceFromCow;
		}

		if (y > 0 || y < 0) {
			//const float halfPi = Math::pi<float>() / 2.0f;
			//float nextRotationOnX = rotation.x - y * rotationDiff;
			//rotation.x = Math::clamp(nextRotationOnX, -halfPi, halfPi);
			double tempYAngle = yAngle + Math::radians(y);
			yAngle = Math::abs(tempYAngle) >= Math::pi<double>() / 2 ? yAngle : tempYAngle;
			position.y = Math::sin(yAngle) * distanceFromCow;
		}

		// UPDATE ORIENTATION
		mCamera.transform.setLocalPosition(position);
		mCamera.transform.lookAt(suzanneTransform.getLocalPosition());
		mCamera.transform.getWorldMatrix();
	};
	

	Renderer renderer;
	Renderer::Mode renderMode[3] = {
		Renderer::Mode::Triangles,
		Renderer::Mode::Lines,
		Renderer::Mode::Points
	};

	uint renderModeIndex = 0;

	bool UsingCCW = true;

	auto changeRenderModeAndFrontFacing = [&]() -> void 
	{
		if (Input::wasReleasedKey(M))
		{
			renderModeIndex = (renderModeIndex + 1) % 3;
			renderer.setRenderMode(renderMode[renderModeIndex]);
		}

		if (Input::wasReleasedKey(C))
		{
			UsingCCW = !UsingCCW;
			if (UsingCCW) 
			{
				renderer.setFrontCounterClockwise();
			}
			else 
			{
				renderer.setFrontClockwise();
			}
		}
	};




	renderer.setFrontCounterClockwise();
	
	//BillboardRenderer billboardRenderer;
	//ShadowRenderer shadowRenderer;


	Light directional;
	directional.direction = Vector3(-1, -1, -1);
	directional.color = Color32(1, 1, 1, 1);
	directional.intensity = 1;
	
	float angle = 0;
	float mx = 0;
	float mz = 0;


	renderer.setClearColor({ 1, 1, 1, 1 });
	// load skybox
	SkyboxRenderer skyboxRenderer;
	skyboxRenderer.setCubeMap(Loader::loadCubeMap("skybox/cloudysky", "png"));
	
	Time::startDeltaTime();

	mWindow.setTitle("Game Loop!");
	MeshRenderer meshRenderer;
	MeshSoftwareRenderer meshSoftwareRenderer;

	// FIRST
	ShaderProgram::build();
	

	meshRenderer.setup();
	meshSoftwareRenderer.setup();
	skyboxRenderer.setup();
	//shadowRenderer.setup();

	uint FPS = 0;
	double SecondsCount = 0;
	
	mWindow.toogleTweakBar();
	
	// TWEAKABLE VARIABLES
	Color32 cowColor = { 1, 1, 1, 1 };
	bool apiOrSoftwareRenderer = true;
	bool hasUploadedToOpenGLVerticesAgain = false;

	typedef enum { Phong, GouraudAD, GouraudADS } ShadingType;
	ShadingType shadingType = Phong;

	TwEnumVal shadingTypesArray[] = { { Phong, "Phong" },{ GouraudAD, "GouraudAD" },{ GouraudADS, "GouraudADS" }};
	TwType shadiningTypeValue = TwDefineEnum("ShadingType", shadingTypesArray, 3);

	TwBar* tweakBar = TwNewBar("TWEAK");

	TwAddSeparator(tweakBar, "Renderer Stuff", "label='Renderer Stuff'");
	TwAddVarRW(tweakBar, "Render Mode", TW_TYPE_BOOL32, &apiOrSoftwareRenderer, "label='Change Render Mode'");
	// Defining season enum type
	
	// Adding season to bar
	TwAddVarRW(tweakBar, "Shading Type", shadiningTypeValue, &shadingType, NULL);

	TwAddSeparator(tweakBar, "Cow Stuff", "label='Cow Stuff'");
	TwAddVarRW(tweakBar, "CowDiffuseColor", TW_TYPE_COLOR4F, &cowColor, "label='Color'");
	
	TwAddSeparator(tweakBar, "Camera Stuff", "label='Camera Stuff'");

	TwAddVarRW(tweakBar, "Reset", TW_TYPE_BOOL32, &shouldReset, "label='Reset'");

	auto VerifyIfShouldReset = [&]()
	{
		if (shouldReset)
		{
			cowColor = { 1, 1, 1, 1 };
			apiOrSoftwareRenderer = true;
			shadingType = Phong;
			shouldReset = false;
		}
	};

	
	std::string renderName = "OpenGL ";

	auto VerifyRenderMode = [&]() -> void
	{
		if (apiOrSoftwareRenderer)
		{
			renderName = "OpenGL";
			if (!hasUploadedToOpenGLVerticesAgain)
			{
				suzanne.getVertexArray().updateBuffer(0, suzanne.getVertices().data(), suzanne.getVertices().size() * sizeof(Vertex));
				hasUploadedToOpenGLVerticesAgain = true;
			}
		}
		else
		{
			renderName = "Close2GL";
			hasUploadedToOpenGLVerticesAgain = false;
		}
	};


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
		//renderer.setRenderMode(Renderer::Mode::Triangles);
		//shadowRenderer.renderShadowMap(mCamera, suzanne, suzanneTransform, directional, renderer);
		//shadowRenderer.renderShadowMap(mCamera, quad, quadTransform, directional, renderer);
		//
		//shadowRenderer.getShadowBuffer().unbind();

		SecondsCount += Time::getDeltaTime();
		FPS += 1;
		if (SecondsCount >= 1)
		{
			mWindow.setTitle(renderName + " - FPS: " + std::to_string(FPS));
			SecondsCount = 0;
			FPS = 0;
		}

		renderer.clearColorAndDepth();
		renderer.setViewport(viewport);

		renderer.setRenderMode(Renderer::Mode::Triangles);
		skyboxRenderer.render(mCamera, renderer);
		//
		renderer.setRenderMode(renderMode[renderModeIndex]);
		meshRenderer.render(mCamera, quad, quadTransform, directional, renderer);
		
		if (apiOrSoftwareRenderer)
		{
			if (shadingType == Phong)
			{
				meshRenderer.usePhong();
			}
			else if (shadingType == GouraudAD)
			{
				meshRenderer.useGouraudAD();
			}
			else if (shadingType == GouraudADS)
			{
				meshRenderer.useGouraudADS();
			}

			meshRenderer.setDiffuseColor(cowColor);
			meshRenderer.render(mCamera, suzanne, suzanneTransform, directional, renderer);
		}
		else
		{
			if (shadingType == Phong)
			{
				meshSoftwareRenderer.usePhong();
			}
			else if (shadingType == GouraudAD)
			{
				meshSoftwareRenderer.useGouraudAD();
			}
			else if (shadingType == GouraudADS)
			{
				meshSoftwareRenderer.useGouraudADS();
			}
			meshSoftwareRenderer.setDiffuseColor(cowColor);
			meshSoftwareRenderer.render(mCamera, suzanne, suzanneTransform, directional, renderer);
		}
		
		//shadowRenderer.renderAdditiveShadow(mCamera, suzanne, suzanneTransform, directional, renderer);
		//shadowRenderer.renderAdditiveShadow(mCamera, quad, quadTransform, directional, renderer);
		//meshRenderer.render(mCamera, suzanne, suzanneTransform, directional, shadowRenderer.getShadowMap(), renderer);
		//meshRenderer.render(mCamera, quad, quadTransform, directional, shadowRenderer.getShadowMap(), renderer);
		
		//particleRenderer.render(mParticleSystem, mCamera, renderer);

		
		//if (shadowRenderer.getShadowBuffer().isComplete())
		//{
		//	renderer.setRenderMode(Renderer::Mode::Quads);
		//	billboardRenderer.render(shadowRenderer.getShadowMap(), { 0.5f, -0.5f, 0.5f, 0.5f }, renderer);
		//}
		
		//billboardRenderer.render(particleTexture, { 0.5f, -0.5f, 0.5f, 0.5f }, renderer);

		
		mWindow.pollEvents();
		Time::updateDeltaTime();
		//updateCameraPositionAndRotation();
		//updateCameraOrientation();	
		updateDistanceFromCow();
		updateCameraOrientationAndPositionLookAtCow();
		changeRenderModeAndFrontFacing();
		
		//resetButtom();
		mWindow.swapBuffers();
		VerifyIfShouldReset();
		VerifyRenderMode();
	}

	// TODO: finish systems and release resources properly
	//GameLoopRunning = false;

	//TwTerminate();
	mWindow.finish();
}
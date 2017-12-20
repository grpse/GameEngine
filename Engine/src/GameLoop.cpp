#include <GL/glew.h>
#include <SDL.h>
#include <iostream>
#include "LinearMath.h"
#include "GameLoop.h"
#include "Time.h"
#include "ParticleSystem.h"
#include "Camera.h"
#include "ParticleMaster.h"
#include "GLErrorHandling.h"

#define RC_INVOKED 1

Uint64 NOW = 0;
Uint64 LAST = 0;
double deltaTimeInSecondsFraction = 0;
double mElapsedSecond = 0;

Window mWindow;
Camera mCamera;
Matrix4 mProjectionMatrix;

float sensitivityX = 15.0;
float sensitivityY = 15.0;

float minimumX = -360.0;
float maximumX = 360.0;

float minimumY = -60.0;
float maximumY = 60.0;

float rotationX = 0;
float rotationY = 0;

float frameCounter = 20;

std::vector<float> rotArrayX;
float rotAverageX = 0;

std::vector<float> rotArrayY;
float rotAverageY = 0;

Quaternion originalRotation;

const float GRAVITY = -9.8f;

void update(ParticleMaster& particleMaster, ShaderProgram& shader);

std::vector<Vbo> objects;
std::vector<Attribute> attributes;

Matrix4 world(1);

void GameLoop::start()
{
	Rect viewport = { 0, 0, 640, 480 };
	mWindow.setViewport(viewport);
	mWindow.start();

	ParticleMaster mParticleMaster;

	float fov = 45.0f;
	float aspectRatio = viewport.width / viewport.height;
	float nearPlane = .001;
	float farPlane = 1000.0;
	mProjectionMatrix = Math::perspective(fov, aspectRatio, nearPlane, farPlane);

	NOW = SDL_GetPerformanceCounter();

	const char* basicShader = R"(
	#BEGIN VERTEXSHADER
	void main() {
		gl_Position = WorldView * vec4(VertexPosition_ModelSpace, 1);
	}	
	#END VERTEXSHADER

	#BEGIN FRAGMENTSHADER
	void main() {
		gl_FragColor = vec4(0, 1, 0, 1);
	}
	#END FRAGMENTSHADER
	)";


	Vbo vbo = Vbo::create(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
	Vector3 quad[] = {
		Vector3(-0.5, -0.5, 0),
		Vector3( 0.5, -0.5, 0),
		Vector3( 0.5,  0.5, 0),
		Vector3(-0.5,  0.5, 0)
	};

	ShaderProgram mShader;
	mShader.useVertexAttribute();
	mShader.useWorldViewMatrix();

	mShader.buildShadersFromSource(basicShader);

	mShader.start();

	vbo.allocate(sizeof(quad));
	vbo.storeData(quad, sizeof(quad), 0);

	objects.push_back(vbo);

	uint VertexPosition_ModelSpace = mShader.getAttributeLocation("VertexPosition_ModelSpace");

	Attribute attrib(VertexPosition_ModelSpace, GL_FLOAT, 3);

	attributes.push_back(attrib);

	mShader.stop();





	mCamera.transform.setLocalPosition({ 0, 0, 10 });
	//mCamera.transform.setLocalRotation(Quaternion(3.1415f/4, 0.0f, 0.0f, 1));
	originalRotation = mCamera.transform.getLocalRotation();

	mParticleMaster.init(mProjectionMatrix);

	ParticleSystem mParticleSystem(50.0, 25, 0.3f, 4);

	mParticleSystem.setParticleMaster(mParticleMaster);

	const uint ArrowUp = 82;
	const uint ArrowDown = 81;
	const uint ArrowLeft = 80;
	const uint ArrowRight = 79;
	const uint Space = 44;

	float rotationInY = 0;

	mWindow.onKeydown([&](uint key) {
		
		//std::cout << "Foi " << key << std::endl;
		if (ArrowUp == key) {
			Vector3 f = mCamera.transform.getFront();
			Vector3 p = mCamera.transform.getLocalPosition();
			f = f * (float)(5.0 * Time::getDeltaTime());
			mCamera.transform.setLocalPosition(p + f);
			mCamera.printPositionAndTarget();
		}
		else if (ArrowDown == key) {
			Vector3 f = mCamera.transform.getFront();
			Vector3 p = mCamera.transform.getLocalPosition();
			f = f * (float)(-5.0 * Time::getDeltaTime());
			mCamera.transform.setLocalPosition(p + f);
			mCamera.printPositionAndTarget();
		}
		else if (ArrowLeft == key) {
			rotationInY += (float)(-50.0 * Time::getDeltaTime());
			mCamera.transform.setLocalRotation(Math::normalize(Vector3(0, rotationInY, 0)));
			mCamera.printPositionAndTarget();
		}
		else if (ArrowRight == key) {
			rotationInY += (float)(50.0 * Time::getDeltaTime());
			mCamera.transform.setLocalRotation(Math::normalize(Vector3(0, rotationInY, 0)));
			mCamera.printPositionAndTarget();
		}
		
		mCamera.transform.getWorldMatrix();

		//std::cout << "Position: " << Math::to_string(mCamera.transform.getLocalPosition()) << std::endl;

		/*
		if (Space == key) {
			mParticleMaster.instantiateOne({ 0, 0, 0 }, Vector3(0, 20, 0), 10, 1, 5, 1);
		}
		*/
	});

	float mouseSpeed = 0.05f;
	float horizontalAngle = 0, verticalAngle = 0;


	mWindow.onMouseMove([&](int x, int y) {

		// Compute new orientation
		horizontalAngle += mouseSpeed * float(viewport.width / 2.0f - x);
		verticalAngle += mouseSpeed * float(viewport.height / 2.0f - y);

		Vector3 direction(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);

		mCamera.transform.setLocalRotation(Quaternion(direction));
		mCamera.transform.getWorldMatrix();
	});

	while (mWindow.isOpen()) {
		
		mParticleSystem.emitParticle(Vector3(0, 0, 0));

		update(mParticleMaster, mShader);

		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		deltaTimeInSecondsFraction = (double)((NOW - LAST) * 1000 / SDL_GetPerformanceFrequency()) / 1000;
		Time::setDeltaTime(deltaTimeInSecondsFraction);
	}

	mWindow.finish();
}

void update(ParticleMaster& particleMaster, ShaderProgram& shader)
{
	// RENDERING
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glClearColor(0.5, 0, 0.5, 0));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	particleMaster.update();

	// 3d stuff


	for (uint i = 0; i < objects.size(); i++) 
	{

		auto& attrib = attributes[i];
		auto& vbo = objects[i];

		shader.start();

		
		Matrix4 view = mCamera.createViewMatrix();
		Matrix4 worldView = view * world;

		/*
		std::cout << "WVP Matrix:: " << std::endl;
		std::cout << Math::to_string(worldViewProjection) << std::endl;
		*/
		shader.setWorldViewMatrix(worldView);
		//shader.setWorldViewProjectionMatrix(Matrix4(1));

		vbo.bind();

		attrib.enable(true);

		attrib.link(0, 0);

		GLCall(glDrawArrays(GL_TRIANGLE_FAN, 0, 4));

		attrib.enable(false);

		vbo.unbind();

		shader.stop();
	}
	


	particleMaster.render(mCamera);

	// 2d stuff

	mWindow.swapBuffers();
	mWindow.pollEvents();
}


const float GameLoop::Gravity()
{
	return GRAVITY;
}

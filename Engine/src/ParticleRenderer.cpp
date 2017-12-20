#include <GL/glew.h>
#include <vector>
#include <iostream>
#include "ParticleRenderer.h"
#include "GLErrorHandling.h"

#define STRINGIFICATOR(value) (#value)

ParticleRenderer::ParticleRenderer()
{
	static Vector3 quadVertices[] = {
		Vector3(-0.5, -0.5, 0),
		Vector3(0.5, -0.5, 0),
		Vector3(0.5, 0.5, 0),
		Vector3(-0.5, 0.5, 0),
	};

	
	//mQuad = Vbo::create(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
	//mQuad.bind();
	//mQuad.allocate(sizeof(quadVertices));
	//mQuad.storeData(&quadVertices[0], sizeof(quadVertices), 0);
	

	GLCall(glGenVertexArrays(1, &mQuadVAO));
	GLCall(glBindVertexArray(mQuadVAO));
	
	GLCall(glGenBuffers(1, &mQuadBuffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, mQuadBuffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW));




	mShader.useVertexAttribute();
	//mShader.useNormalAttribute();
	//mShader.useTextureCoord0Attribute();

	mShader.useWorldViewMatrix();
	mShader.useProjectionMatrix();


	const char* ParticleShaderStr = R"(
		#BEGIN VERTEXSHADER																		
		void main() {																			
			gl_Position = Projection * WorldView * vec4(VertexPosition_ModelSpace, 1); 			
			//gl_Position = vec4(VertexPosition_ModelSpace, 1); 			
		}																						
		#END VERTEXSHADER
																	
		#BEGIN FRAGMENTSHADER																	
		void main() {																			
			gl_FragColor = vec4(1); 														
		}																						
		#END FRAGMENTSHADER																	
	)";

	mShader.buildShadersFromSource(ParticleShaderStr);
}

ParticleRenderer::~ParticleRenderer()
{

}

void ParticleRenderer::init(const Matrix4 & projection)
{
	mProjection = projection;

	mShader.start();
	mShader.setProjectionMatrix(mProjection);

	GLCall(glBindVertexArray(mQuadVAO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, mQuadBuffer));

	uint vertexAttribute = mShader.getAttributeLocation("VertexPosition_ModelSpace");
	mQuadVertAttributeLocation = vertexAttribute;
	//mQuadAttributes = Attribute(vertexAttribute, GL_FLOAT, 2);

	// 1rst attribute buffer : vertices
	GLCall(glEnableVertexAttribArray(mQuadVertAttributeLocation));
	GLCall(glVertexAttribPointer(
		mQuadVertAttributeLocation,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	));
	
	GLCall(glDisableVertexAttribArray(vertexAttribute));

	mShader.stop();
}

void ParticleRenderer::render(const Particle particles[], uint particleCount, const Camera & camera)
{
	Matrix4 view = Matrix4(1);// camera.createViewMatrix();
	prepare();

	while(particleCount--) {
		Particle particle = particles[particleCount];
		updateModelViewMatrix(particle.getPosition(), particle.getRotation(), particle.getScale(), view);

		// 1rst attribute buffer : vertices
		GLCall(glEnableVertexAttribArray(mQuadVertAttributeLocation));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, mQuadBuffer));
		GLCall(glVertexAttribPointer(
			mQuadVertAttributeLocation,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		));

		GLCall(glDrawArrays(GL_QUADS, 0, 4));

		GLCall(glDisableVertexAttribArray(mQuadVertAttributeLocation));
	}

	finishRendering();
}

void ParticleRenderer::updateModelViewMatrix(const Vector3& position, float rotation, float scale, const Matrix4& view)
{
	Matrix4 world = Math::translate(position);
	
	// clear rotation on matrix multiplication
	world[0][0] = view[0][0];
	world[1][0] = view[0][1];
	world[2][0] = view[0][2];
	
	world[0][1] = view[1][0];
	world[1][1] = view[1][1];
	world[2][1] = view[1][2];

	world[0][2] = view[2][0];
	world[1][2] = view[2][1];
	world[2][2] = view[2][2];

	// rotate (camera facing) only in z
	world = Math::rotate(world, Math::radians(rotation), Vector3(0, 0, 1));
	world = Math::scale(world, Vector3(scale, scale, scale));

	// get world view matrix
	Matrix4 worldView = view * world;

	mShader.setWorldViewMatrix(worldView);
}

void ParticleRenderer::prepare()
{
	mShader.start();
	
	GLCall(glBindVertexArray(mQuadVAO));
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCall(glDepthMask(GL_FALSE));
}

void ParticleRenderer::finishRendering()
{
	GLCall(glDepthMask(GL_TRUE));
	GLCall(glDisable(GL_BLEND));
	GLCall(glBindVertexArray(0));	
	mShader.stop();
}

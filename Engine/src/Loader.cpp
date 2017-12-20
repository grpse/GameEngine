#include <GL/glew.h>
#include "Loader.h"
#include "GLErrorHandling.h"

Loader::Loader() 
{

}

Loader::~Loader()
{
	for (auto vao : mLoadedVAOS)
	{
		glDeleteVertexArrays(1, &vao);
	}

	for (auto vbo : mLoadedVBOs)
	{
		glDeleteBuffers(1, &vbo);
	}
}

RawModel Loader::loadToVAO(void* data, uint itemSizeInBytes, uint itemCount)
{
	uint vao = createVAO();
	storeDataInAttributeList(0, data, itemSizeInBytes * itemCount);
	return RawModel(vao, itemCount);
}

uint Loader::createVAO()
{
	uint vao;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	mLoadedVAOS.push_back(vao);

	return vao;
}

void Loader::storeDataInAttributeList(int attributeNumber, void * data, uint sizeInBytes)
{
	uint vbo;
	GLCall(glGenBuffers(1, &vbo));

	mLoadedVBOs.push_back(vbo);

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeInBytes, data, GL_STATIC_DRAW));
	GLCall(glVertexAttribPointer(attributeNumber, 3, GL_FLOAT, false, 0, (void*)0));

	// unbind current vbo
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void Loader::unbindVAO()
{
	GLCall(glBindVertexArray(0));
}

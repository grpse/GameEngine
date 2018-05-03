#include <GL/glew.h>
#include "VertexArray.h"
#include "GLErrorHandling.h"
#include "ShaderProgram.h"

VertexArray::VertexArray() 
	: mWasGenerated(false)
{

}

VertexArray::VertexArray(const VertexArray & other)
{
	mID = other.mID;
	mWasGenerated = false;
}

VertexArray::~VertexArray() 
{

}

void VertexArray::bind() const
{
	GLCall(glBindVertexArray(mID));
	setupLayout();
}

void VertexArray::unbind() const
{
    GLCall(glBindVertexArray(0));
}

void VertexArray::addVertexBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    mVertexBufferLayouts.push_back(layout);
	mVertexBuffers.push_back(vb);
	mVBOs.push_back(vb.mID);
}

void VertexArray::updateBuffer(uint offset, const void* data, size_t size) const
{
	GLCall(glBindVertexArray(mID));
	for (uint vbo : mVBOs)
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	}
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
}

void VertexArray::generateBuffer()
{
	if (!mWasGenerated)
	{
		mWasGenerated = true;
		GLCall(glGenVertexArrays(1, &mID));
	}

	GLCall(glBindVertexArray(mID));
}

void VertexArray::deleteBuffer()
{
	GLCall(glDeleteVertexArrays(1, &mID));
}

void VertexArray::setupLayout() const
{
	if (!mWasSetLayout)
	{
		GLCall(glBindVertexArray(mID));

		for (uint i = 0; i < mVertexBufferLayouts.size(); i++)
		{
			auto& mVertexBuffer = mVertexBuffers[i];
			auto& mVertexBufferLayout = mVertexBufferLayouts[i];

			mVertexBuffer.bind();
		
			auto& elements = (std::vector<VertexBufferElement>&)mVertexBufferLayout.getElements();
			uint offset = 0;

			for (uint j = 0; j < elements.size(); j++)
			{
				auto& element = elements[j];
				element.attributeLocation = ShaderProgram::getCurrentBound().getAttributeLocation(element.attributeName);
				std::cout << "Attribute Name: " << element.attributeName << ", Attribute Location: " << element.attributeLocation << std::endl;

				// push only elements that have attributes on shader program

				// Setup memory layout only for elements have their presence on shader program
				if (element.attributeLocation >= 0)
				{
					GLCall(glEnableVertexAttribArray(element.attributeLocation));
					GLCall(glVertexAttribPointer(
						element.attributeLocation,
						element.count,
						element.type,
						element.normalized,
						mVertexBufferLayout.getStride(),
						(const void*)offset
					));
				}

				offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
			}

			mVertexBuffer.unbind();
			//GLCall(glBindVertexArray(0));
		}

		mVertexBuffers.clear();
		mVertexBufferLayouts.clear();
		mWasSetLayout = true;
	}
}
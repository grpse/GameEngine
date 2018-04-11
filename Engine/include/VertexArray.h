#pragma once

#include "Typedefs.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include <vector>

class Renderer;

class VertexArray {
public:
    VertexArray();
	VertexArray(const VertexArray& other);
    ~VertexArray();

    void addVertexBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void generateBuffer();
	void deleteBuffer();


private:
    uint mID;
	bool mWasGenerated;
	mutable bool mWasSetLayout = false;
    mutable std::vector<VertexBufferLayout> mVertexBufferLayouts;
	mutable std::vector<VertexBuffer> mVertexBuffers;

	void setupLayout() const;
    void bind() const;
    void unbind() const;

	friend class Renderer;
};
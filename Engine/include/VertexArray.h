#pragma once

#include "Typedefs.h"
#include "VertexBufferLayout.h"

class VertexBuffer;

class VertexArray {
public:
    VertexArray();
	VertexArray(const VertexArray& other);
    ~VertexArray();

    void setVertexBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void generateBuffer();
	void deleteBuffer();

    void bind() const;
    void unbind() const;

private:
    uint mID;
	bool mWasGenerated;
    VertexBufferLayout mVertexBufferLayout;

};
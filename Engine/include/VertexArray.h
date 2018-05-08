#pragma once

#include "Typedefs.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include <vector>
#include "OSExport.h"

class Renderer;

class ENGINE_API VertexArray {
public:
    VertexArray();
	VertexArray(const VertexArray& other);
    ~VertexArray();

	template <typename Type>
	void createVertexBuffer(const void* data, uint count, const VertexBufferLayout& layout)
	{
		generateBuffer();
		VertexBuffer vbo;
		vbo.load<Type>(data, count);
		addVertexBuffer(vbo, layout);
	}

	template <typename Type>
	void createDynamicVertexBuffer(const void* data, uint count, const VertexBufferLayout& layout)
	{
		generateBuffer();
		VertexBuffer vbo;
		vbo.loadDynamic<Type>(data, count);
		addVertexBuffer(vbo, layout);
	}

	void updateBuffer(uint offset, const void* data, size_t size) const;


private:
    uint mID;
	bool mWasGenerated;
	mutable bool mWasSetLayout = false;
    mutable std::vector<VertexBufferLayout> mVertexBufferLayouts;
	mutable std::vector<VertexBuffer> mVertexBuffers;
	mutable std::vector<uint> mVBOs;

    void addVertexBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void generateBuffer();
	void deleteBuffer();

	void setupLayout() const;
    void bind() const;
    void unbind() const;

	friend class Renderer;
};
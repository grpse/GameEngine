#pragma once

#include "Typedefs.h"

class VertexBuffer {

public:
    VertexBuffer();
	VertexBuffer(const VertexBuffer& other);
    ~VertexBuffer();

	template<typename Type>
	void load(const void* data, uint count)
	{
		load(data, count * sizeof(Type));
	}

	template<typename Type>
	void loadDynamic(const void* data, uint count)
	{
		loadDynamic(data, count * sizeof(count));
	}

    void bind() const;
    void unbind() const;

private:
    uint mID;

    void load(const void* data, uint size);
	void loadDynamic(const void* data, uint size);

	friend class VertexArray;
};
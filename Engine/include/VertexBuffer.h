#pragma once

#include "Typedefs.h"


class VertexBuffer {

public:
    VertexBuffer();
    VertexBuffer(const void* data, uint size);
    ~VertexBuffer();

	template<typename Type>
	void load(const void* data, uint count)
	{
		load(data, count * sizeof(Type));
	}

    void load(const void* data, uint size);

    void bind() const;
    void unbind() const;

private:
    uint mID;

};
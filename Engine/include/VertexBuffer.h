#pragma once

#include "Typedefs.h"


class VertexBuffer {

public:
    VertexBuffer();
    VertexBuffer(const void* data, uint size);
    ~VertexBuffer();

    void load(const void* data, uint size);

    void bind() const;
    void unbind() const;

private:
    uint mID;

};
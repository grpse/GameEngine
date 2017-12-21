#pragma once

#include "Typedefs.h"


class IndexBuffer {

public:
    IndexBuffer(const uint* data, uint count);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;

    inline uint getElementCount() const {
        return mCount;
    }

private:
    uint mID;
    uint mCount;
};
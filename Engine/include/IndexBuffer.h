#pragma once

#include "Typedefs.h"


class IndexBuffer {

public:
    IndexBuffer();
    IndexBuffer(const uint* data, uint count);
    ~IndexBuffer();

    void load(const uint* data, uint count);

    void bind() const;
    void unbind() const;

    inline uint getElementCount() const {
        return mCount;
    }

private:
    uint mID;
    uint mCount;
};
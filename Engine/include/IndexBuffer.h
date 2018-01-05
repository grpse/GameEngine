#pragma once

#include "Typedefs.h"


class IndexBuffer {

public:
    IndexBuffer();
	IndexBuffer(const IndexBuffer& other);
    IndexBuffer(const uint* data, uint count);
    ~IndexBuffer();

	void generateBuffer();
	void deleteBuffer();

    void load(const uint* data, uint count);

    void bind() const;
    void unbind() const;

    inline uint getElementCount() const {
        return mCount;
    }

private:
    uint mID;
    uint mCount;

	bool mWasGenerated;
};
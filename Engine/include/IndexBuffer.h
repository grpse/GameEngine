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

	template<typename Type>
	inline void load(const void* data, uint count)
	{
		loadData(data, count, sizeof(Type));
	}

	void loadData(const void* data, size_t count, size_t typeSize);
    void load(const uint* data, uint count);

    void bind() const;
    void unbind() const;

    inline uint getElementCount() const 
	{
        return mCount;
    }

private:
    uint mID;
    uint mCount;

	bool mWasGenerated;
};
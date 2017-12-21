#pragma once
#include "Typedefs.h"

class ParticleTexture{

public:
    ParticleTexture() {

    }

    void setup(uint textureID, uint numberOfRows) {
        mTextureID = textureID;
        mNumberOfRows = numberOfRows;
    }

    inline uint getTextureID() const {
        return mTextureID;
    }

    inline uint getNumberOfRows() const {
        return mNumberOfRows;
    }

private:
    uint mTextureID;
    uint mNumberOfRows;
};
#pragma once

#include <GL/glew.h>
#include "Typedefs.h"

class Texture2D {

public:

    Texture2D();
    Texture2D(uint id);
    Texture2D(const Texture2D& other);

    void setIndex(uint index);
    uint getIndex() const;
    void setFormat(uint format);
    uint getFormat() const;
    void setParameter(uint name, uint value);
    void loadData(const void* data, uint width, uint height);
    void generateMipMaps();
    void start() const;
    void stop() const;
    uint getWidth() const;
    uint getHeight() const;

    Texture2D& operator=(const Texture2D& other);
private:
    uint mID;
    uint mIndex;
    uint mWidth;
    uint mHeight;
    uint mFormat; 
        //GL_RED, GL_RED_INTEGER, GL_RG, 
        //GL_RG_INTEGER, GL_RGB, GL_RGB_INTEGER, 
        //GL_RGBA, GL_RGBA_INTEGER, GL_DEPTH_COMPONENT, 
        //GL_DEPTH_STENCIL, GL_LUMINANCE_ALPHA, 
        //GL_LUMINANCE, and GL_ALPHA.

    void setupDefaults();
};
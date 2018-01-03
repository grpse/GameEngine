#include <GL/glew.h>
#include <SOIL.h>
#include "Loader.h"
#include "GLErrorHandling.h"
Loader::Loader() 
{

}

Loader::~Loader()
{
	
}

Texture2D Loader::loadRGBATexture2D(const char* filepath)
{
	int width, height, channels;
	byte* imageData = SOIL_load_image(filepath, &width, &height, &channels, SOIL_LOAD_RGBA);

	Texture2D texture;
	texture.start();
	texture.setFormat(GL_RGBA);
	texture.loadData(imageData, width, height);
	texture.stop();

	SOIL_free_image_data(imageData);

	return texture;
}
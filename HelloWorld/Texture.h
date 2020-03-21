#pragma once
#include <GL/glew.h>
#include "stb_image.h"
#include <stdio.h>

class Texture {
private:
	int height;
	int width;
	int bitDepth;
	GLuint textureId;
	char* fileLocation;

public:
	Texture();
	Texture(const char* fileLoc);

	void loadTexture();
	void useTexture();
	void clearTexture();


	~Texture();
};
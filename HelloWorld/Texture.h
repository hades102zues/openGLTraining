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
	Texture(char* fileLoc);

	void loadTexture();

	~Texture();
};
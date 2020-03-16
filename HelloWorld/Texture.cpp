#include "Texture.h"

Texture::Texture() {
	textureId = 0;
	width = height = bitDepth = 0;
	fileLocation = nullptr;
}

Texture::Texture(char* fileLoc) {
	textureId = 0;
	width = height = bitDepth = 0;
	fileLocation = fileLoc;
}




void Texture::loadTexture() {
	const char* textureData = 
}

Texture::~Texture() {

}
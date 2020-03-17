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

	//use stb to load the data
	unsigned char* textureData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
	
	if (!textureData) {
		printf("Error Loading texture.");
	}

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	//******set some texture handling properties
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//attach texture data to texture object
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);

	//automatically generate mipmap
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	//unload texture Data
	stbi_image_free(textureData);
}

Texture::~Texture() {

}
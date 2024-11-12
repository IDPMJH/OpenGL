#include "BitmapHandler.h"
#define STB_IMAGE_IMPLEMENTATION  
#include <stb_image.h>

GLuint BitmapHandler::loadTexture(const char* path) {

	GLuint textureID = -1;
	int width, height, channels;

	//textury
	unsigned char* image = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);
	if (image == nullptr)
	{
		printf("Nie uda�o si?wczyta?tekstury\n");
		return textureID;
	}

	// Utworzenie bufora tekstury
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Przypisanie obrazka do bufora tekstury
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	// Zwolnienie pami?i zajmowanej przez obrazek
	stbi_image_free(image);

	// Ustawienie parametr? tekstury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return textureID;
}


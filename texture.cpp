#include "texture.h"
#include <GL/glew.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using std::cerr;
using std::endl;

Texture::Texture() {}

Texture Texture::from_image(string filename) {
	Texture texture = Texture();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, num_channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(
		filename.c_str(), 
		&width, 
		&height, 
		&num_channels, 
		0
	);
	if (!data) cerr << "Could not load image: " << filename << endl;

	unsigned int texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id); // Stays bound for drawing
	GLenum format = GL_RGB;
	if (num_channels == 4) format = GL_RGBA; 
	glTexImage2D(
		GL_TEXTURE_2D, 
		0, 
		format, 
		width, 
		height, 
		0, 
		format, 
		GL_UNSIGNED_BYTE, 
		data
	);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	texture.texture_id = texture_id;
	return texture;
}
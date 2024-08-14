#include "Texture.h"

Texture::Texture(const char* image_path, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
	type = texType;

	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(image_path, &widthImg, &heightImg, &numColCh, 0);

	// Generates a texture object name and returns it inside the ID
	glGenTextures(1, &ID);

	// Activate the texture slot and assings the texture to the Texture unit
	glActiveTexture(slot);
	glBindTexture(texType, ID);

	// Configures the type of the algorithm which is used to make image smaller or bigger
	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// configures the way the texture repeats (if it does at all)
	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Texturing allows elements of an image array to be read by shaders.
	glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(texType);

	// Deletes the image data since it is already in the OpenGL Texture object
	stbi_image_free(bytes);
	glBindTexture(texType, 0);
}

void Texture::texUnit(Shader shader, const char* uniform, GLuint unit)
{
	// Gets ID of uniform called "texCoord"
	GLuint tex0Uni = glGetUniformLocation(shader.ID, uniform);

	shader.Activate();
	glUniform1i(tex0Uni, unit);
}

void Texture::Bind()
{
	glBindTexture(type, ID);
}

void Texture::Unbind()
{
	glBindTexture(type, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}

#include "Texture.h"

Texture::Texture(const char* image_path, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
	type = texType;

	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(image_path, &widthImg, &heightImg, &numColCh, 0);

	// Generate a texture object and assings a reference to that object
	GLuint texture;
	glGenTextures(1, &ID);

	// Activate the texture slot and bind it
	glActiveTexture(slot);
	glBindTexture(texType, ID);

	// Adjust texture settings
	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Generate a texture based on a binary info from the file.png
	glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(texType);

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

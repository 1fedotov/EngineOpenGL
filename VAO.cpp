#include"VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

// Links a VBO to the VAO using a certain layout
void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	// Configure the Vertex Attribute so that OpenGL knows how to read the VBO bytes
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	// Enable the Vertex Attribute so that OpenGL start to use it
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

void VAO::Bind()
{
	glBindVertexArray(ID);
}

void VAO::Unbind()
{
	glBindVertexArray(0);
}

void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}
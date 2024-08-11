#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Imports the color from Vertex Shader
in vec3 color;

// Imports the texture coord from the Vertex Shader
in vec2 texCoord;

// Uniform will tell OpenGL which texture unit to use
uniform sampler2D tex0;

void main()
{
	FragColor = texture(tex0, texCoord);
}
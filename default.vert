#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aColor;
// Textures
layout (location = 2) in vec2 aTex;

// Outputs the color for the Fragment Shader
out vec3 color;

// Outputs texture coordinates to the Fragment Shader
out vec2 texCoord;

// Controls the scale of the vertices
uniform float scale;

void main()
{
	gl_Position = vec4(aPos.x + aPos.x * scale, aPos.y + aPos.y * scale, aPos.z + aPos.z * scale, 1.0);
	// Assings the colors from the Vertex Data to "color"
	color = aColor;
	texCoord = aTex;
}
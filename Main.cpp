#define GLFW_INCLUDE_NONE
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>

#include"Shader.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 800

// Array of vertices coordinates, 3 vertices
GLfloat vertices[] =
{
	-0.5f, -0.5f,  0.0f,  1.0f, 0.0f,  0.0f, // Lower left corner
	-0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, // Upper left corner
	 0.5f,  0.5f,  0.0f,  0.0f, 0.0f,  1.0f, // Upper right corner
	 0.5f, -0.5f,  0.0f,  1.0f, 1.0f,  1.0f, // Lower right corner
};

GLuint indices[] =
{
	0, 2, 1, // Upper triangle
	0, 3, 2 // Lower triangle
};

// Error callback for OpenGL
void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

int main()
{
	// Setting an error callback
	glfwSetErrorCallback(error_callback);

	// Initialize GLFW library
	if (!glfwInit())
	{
		std::cout << "Can't initialize GLFW" << std::endl;
		return -1;
	}

	// Hint to GLFW what OpenGL version we are going to use
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Hint to GLFW to use he CORE profile of OpenGL
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creates a combined window/context object and save it's address
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "EngineOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Making the OpenGL context current
	glfwMakeContextCurrent(window);

	// Loads glad so it configures OpenGL
	gladLoadGL();

	// Specify window area for OpenGL to render
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	Shader shaderProgram("default.vert", "default.frag");

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Gets ID of uniform called "scale"
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	// Reading the timer
	double time = glfwGetTime();

	// Number of screen updates to wait from the time glfwSwapBuffers 
	// was called before swapping the buffers and returning
	glfwSwapInterval(1);

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Set a color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

		// Fill the back buffer with the color 
		glClear(GL_COLOR_BUFFER_BIT);

		// Specify for OpenGL which shader program to use
		shaderProgram.Activate();
		// Assigns a value to the uniform; MUST always be done after shader activation
		glUniform1f(uniID, 0.5f);

		// Bind the VAO so OpenGL could use it
		VAO1.Bind();

		// Draw the triangle using GL_TRIANGLES primitives
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Swap front buffer with back buffer
		glfwSwapBuffers(window);

		// Processing GLFW events
		glfwPollEvents();
	}

	// Delete all the object before exiting the program
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	// Delete window before exiting program
	glfwDestroyWindow(window);

	// Destroys any remaining windows and releases any other resources allocated by GLFW
	glfwTerminate();
	return 0;
}
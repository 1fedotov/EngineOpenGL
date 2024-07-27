#define GLFW_INCLUDE_NONE
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>


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
	GLFWwindow* window = glfwCreateWindow(1600, 800, "EngineOpenGL", NULL, NULL);
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
	glViewport(0, 0, 1600, 800);

	// Set a color
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// Fill the back buffer with the color 
	glClear(GL_COLOR_BUFFER_BIT);
	// Swap front buffer with back buffer
	glfwSwapBuffers(window);

	// Reading the timer
	double time = glfwGetTime();

	// Number of screen updates to wait from the time glfwSwapBuffers 
	// was called before swapping the buffers and returning
	glfwSwapInterval(1);

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Processing GLFW events
		glfwPollEvents();
	}


	// Delete window before exiting programm
	glfwDestroyWindow(window);

	// Destroys any remaining windows and releases any other resources allocated by GLFW
	glfwTerminate();
	return 0;
}
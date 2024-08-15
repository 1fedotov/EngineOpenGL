#define GLFW_INCLUDE_NONE
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Shader.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Texture.h"

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 800

// Array of vertices coordinates, 3 vertices
GLfloat vertices[] =
{ //     COORDINATES			COLORS		      TEXTURES
	-0.5f,  0.0f,  0.5f,  0.83f, 0.70f,  0.44f,  0.0f, 0.0f,
	-0.5f,  0.0f, -0.5f,  0.83f, 0.70f,  0.44f,  5.0f, 1.0f,
	 0.5f,  0.0f, -0.5f,  0.83f, 0.70f,  0.44f,  0.0f, 0.0f,
	 0.5f,  0.0f,  0.5f,  0.83f, 0.70f,  0.44f,  5.0f, 0.0f, 
	 0.0f,  0.8f,  0.0f,  0.92f, 0.86f,  0.76f,  2.5f, 5.0f 
};

GLuint indices[] =
{
	0, 2, 1, 
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};

// Error callback for OpenGL
void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
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

	// Links VBO attributes such as coordinates, colors and texture coords to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0); // coordinates
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float))); // colors
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float))); // texture coords
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Gets ID of uniform called "scale"
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	// Handle TEXTURES
	
	Texture marble("seamless_brick-512x512.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	marble.texUnit(shaderProgram, "tex0", 0);


	float rotation = 0.0f;
	// Reading the timer
	double time = glfwGetTime();
	
	// Number of screen updates to wait from the time glfwSwapBuffers 
	// was called before swapping the buffers and returning
	glfwSwapInterval(1);

	glEnable(GL_DEPTH_TEST);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Process Input
		ProcessInput(window);

		//////////////////////////////////////
		//Process Frame
		/////////////////////////////////////
		// Set a color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Fill the back buffer with the color 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Specify for OpenGL which shader program to use
		shaderProgram.Activate();

		rotation += 0.5f;

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
		proj = glm::perspective(glm::radians(45.0f), (float)(WINDOW_WIDTH / WINDOW_HEIGHT), 0.1f, 100.0f);

		int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		// Assigns a value to the uniform; MUST always be done after shader activation
		glUniform1f(uniID, 0.5f);
		marble.Bind();
		// Bind the VAO so OpenGL could use it
		VAO1.Bind();
		// Draw the triangle using GL_TRIANGLES primitives
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
		// Swap front buffer with back buffer
		glfwSwapBuffers(window);
		//////////////////////////////////////

		// Processing GLFW events
		glfwPollEvents();
	}

	// Delete all the object before exiting the program
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	marble.Delete();
	shaderProgram.Delete();

	// Delete window before exiting program
	glfwDestroyWindow(window);

	// Destroys any remaining windows and releases any other resources allocated by GLFW
	glfwTerminate();
	return 0;
}
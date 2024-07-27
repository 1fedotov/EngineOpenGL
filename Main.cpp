#define GLFW_INCLUDE_NONE
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 800

// Vertex shader
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
// Fragment shader
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

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

	// Array of vertices coordinates, 3 vertices
	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,  // Lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f // Upper center corner
	};

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

	// Create Vertex Shader Object and get reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Compile the Vertex Shader code into machine code
	glCompileShader(vertexShader);

	// Create Fragment Shader Object and get reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to the Vertex Shader Object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// Compile the Fragment Shader code into machine code
	glCompileShader(fragmentShader);

	// Create Shader Program and get reference
	GLuint shaderProgram = glCreateProgram();
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(shaderProgram);

	// Delete the now useless Vertex and Fragment Shader Objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// Create reference containers for the Vertex Array Object
	// and Vertex Buffer Object
	GLuint VAO, VBO;
	// Generate the VAO
	glGenVertexArrays(1, &VAO);
	// Generate the VBO
	glGenBuffers(1, &VBO);

	// Make the VAO as the current Vertex Array object by binding it
	glBindVertexArray(VAO);

	// Bind the VBO specifying it's a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Introduce the vertices into the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Configure the Vertex Attribute so that OpenGL knows how to read the VBO bytes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// Enable the Vertex Attribute so that OpenGL start to use it
	glEnableVertexAttribArray(0);

	// Bind both the VBO and VAO to 0 so that we won't be able
	// accidentally modify them
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


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
		glUseProgram(shaderProgram);

		// Bind the VAO so OpenGL could use it
		glBindVertexArray(VAO);

		// Draw the triangle using GL_TRIANGLES primitives
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap front buffer with back buffer
		glfwSwapBuffers(window);

		// Processing GLFW events
		glfwPollEvents();
	}

	// Delete all the object before exiting the program
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// Delete window before exiting program
	glfwDestroyWindow(window);

	// Destroys any remaining windows and releases any other resources allocated by GLFW
	glfwTerminate();
	return 0;
}
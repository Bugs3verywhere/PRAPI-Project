#include <iostream>
#include <fstream> //reads files
#include <string>
#include <sstream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGLFiles/cpp/Shader.h"
#include "OpenGLFiles/cpp/Renderer.h"
#include "OpenGLFiles/cpp/Window.h"

int main() {
	glfwInit();

	Window window(800, 600, "Test Test");

	if (!window.GetNativeWindow()) {
		return EXIT_FAILURE;
	}

	int screenWidth, screenHeight;
	glfwGetFramebufferSize(
		window.GetNativeWindow(),
		&screenWidth,
		&screenHeight
	);

	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, screenWidth, screenHeight);

	float verticesData[] = {
		//square front face vertices
			-0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,
			-0.5f, 0.5f, -0.5f, 	0.0f, 0.0f, 1.0f,
			0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,

		//square back face vertices
			-0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,
			-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,
			0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 1.0f
	}; //cube vertex positions and colour data, x first y second then rbg data

	unsigned int indices[] = {

		0, 2, 3,
		0, 3, 1, //front

		4, 5, 7,
		4, 7, 6, //back

		0, 4, 6,
		0, 6, 2, //left

		1, 3, 7,
		1, 7, 5, //right

		2, 6, 7,
		2, 7, 3, //top

		0, 1, 5,
		0, 5, 4, //bottom
	};

	Renderer renderer;

	renderer.Initialize(verticesData, sizeof(verticesData), indices, sizeof(indices));
	
	ShaderProgramSource source = ParseShader("OpenGLFiles/shaders/basic.shader"); //tells the shader command what shader file to use
	//std::cout << source.VertexSource << std::endl;
	//std::cout << source.FragmentSource << std::endl;

	glm::mat4 trans = glm::mat4(1.0f); //makes a matrix that we have named transform
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f)); //creates a "camera" by pushing the scene back by 3 
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(screenWidth) / float(screenHeight), 0.1f, 100.0f); //creates a projection with a 45 degree FOV, the size of the window and clipping planes of 0.1 to 100


	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	//window main loop
	while (!window.ShouldClose()) {

		renderer.Clear();

		trans = glm::mat4(1.0f); //reset the transform matrix
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
		unsigned int transformLoc = glGetUniformLocation(shader, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
		unsigned int viewLoc = glGetUniformLocation(shader, "view");
		glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
		unsigned int projLoc = glGetUniformLocation(shader, "projection");
		glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		renderer.Draw(36);
		window.SwapBuffers();
		window.PollEvents();
	}

	glDeleteProgram(shader);
	glfwTerminate();
	return EXIT_SUCCESS;
}
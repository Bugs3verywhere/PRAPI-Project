#define GLEW_STATIC

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <curl/curl.h>
#include <calceph.h>
#include <novas-calceph.h>
#include <supernovas.h>
#include <vector>

#include "OpenGLFiles/source/Shader.h"
#include "OpenGLFiles/source/Renderer.h"
#include "OpenGLFiles/source/Window.h"
#include "OpenGLFiles/source/Camera.h"
#include "OpenGLFiles/source/Object.h"

using namespace supernovas;
using namespace std;

float theta = 0;
float phi = 0;
float radius = 2;
int currentRing = 0;
int currentPoint = 0;

float x;
float y;
float z;

unsigned int indexA;
unsigned int indexB;
unsigned int indexC;
unsigned int indexD;

vector<glm::vec3> pointsVector;
vector<unsigned int> indicesVector;

int main() {

	
	glfwInit();

	Object objectHandler;

	if (!objectHandler.initialiseEphemeris())
		return EXIT_FAILURE;

	vector<ObjectValues> renderObject = objectHandler.createObjects();
	objectHandler.calculatePositions(renderObject);

	Window window(1280, 720, "Test Test");

	if (!window.GetNativeWindow()) {
		return EXIT_FAILURE;
	}

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window.GetNativeWindow(), &screenWidth, &screenHeight);

	if (GLEW_OK != glewInit()) {
		return EXIT_FAILURE;
	}

	glViewport(0, 0, screenWidth, screenHeight);

	//Sphere point generation

	for (phi = 0; phi <= 180; phi += 10)
	{
		for (theta = 0; theta < 360; theta += 10)
		{
			x = (radius * glm::sin(glm::radians(phi)) * glm::cos(glm::radians(theta)));
			y = (radius * glm::cos(glm::radians(phi)));
			z = (radius * glm::sin(glm::radians(phi)) * glm::sin(glm::radians(theta)));
			glm::vec3 position(x, y, z);
			pointsVector.push_back(position);
		}
	}

	for (currentRing = 0; currentRing < 18; currentRing++)
	{
		for (currentPoint = 0; currentPoint < 36; currentPoint++)
		{
			indexA = (currentRing * 36) + currentPoint;
			indexB = (currentRing * 36) + (currentPoint + 1) % 36;
			indexC = ((currentRing + 1) * 36) + currentPoint;
			indexD = ((currentRing + 1) * 36) + ((currentPoint + 1) % 36);

			//indice 1
			indicesVector.push_back(indexA);
			indicesVector.push_back(indexB);
			indicesVector.push_back(indexD);

			//indice 2
			indicesVector.push_back(indexA);
			indicesVector.push_back(indexD);
			indicesVector.push_back(indexC);

		}
	}

	Renderer renderer;
	renderer.Initialize(pointsVector.data(), pointsVector.size() * sizeof(glm::vec3), indicesVector.data(), indicesVector.size() * sizeof(unsigned int));

	Camera camera(glm::vec3(0, 0, 5));
	
	ShaderProgramSource source = ParseShader("OpenGLFiles/shaders/basic.shader");
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader);

	glm::vec3 position = glm::vec3(0, 0, 5);
	glm::mat4 projection = glm::perspective(glm::radians(camera.GetFoV()), float(screenWidth) / float(screenHeight), 0.1f, 100.0f);

	glDisable(GL_CULL_FACE);

	//window main loop
	while (!window.ShouldClose()) {

		renderer.Clear();

		unsigned int viewLoc = glGetUniformLocation(shader, "view");
		camera.UpdateCamera(window.GetNativeWindow(), screenWidth, screenHeight);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

		unsigned int projLoc = glGetUniformLocation(shader, "projection");
		glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


		unsigned int transformLoc = glGetUniformLocation(shader, "transform");
		unsigned int planetColour = glGetUniformLocation(shader, "planetColour");


		for (ObjectValues& currentObject : renderObject)
		{
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(currentObject.objectTransform));
			glUniform4fv(planetColour, 1, glm::value_ptr(currentObject.objectColour));
			renderer.Draw(indicesVector.size());
		}

		window.SwapBuffers();
		window.PollEvents();
	}

	glDeleteProgram(shader);
	glfwTerminate();
	return EXIT_SUCCESS;
}
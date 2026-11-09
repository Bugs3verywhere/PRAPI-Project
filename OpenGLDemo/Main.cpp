#define GLEW_STATIC

#include <iostream>
#include <fstream>
#include <string>
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

#include "OpenGLFiles/cpp/Shader.h"
#include "OpenGLFiles/cpp/Renderer.h"
#include "OpenGLFiles/cpp/Window.h"
#include "OpenGLFiles/cpp/Camera.h"

using namespace supernovas;
using namespace std;

int main() {

	t_calcephbin* ephemeris =
		calceph_open("OpenGLFiles/objectephemeris/de440s.bsp");

	calceph_prefetch(ephemeris);

	int status = novas_use_calceph_planets(ephemeris);

	std::cout << "CALCEPH registration: " << status << '\n';

	auto planetSource = Planet::jupiter();
	Site site = Site::from_GPS(-1.087 * Unit::deg, 50.805 * Unit::deg, 60.0 * Unit::m);
	EOP eop(37, 0.6447 * Unit::s, 103.2 * Unit::mas, 211.3 * Unit::mas);
	auto obs = site.to_observer(eop);
	Time obs_time = Time::now(eop);
	Frame frame = obs.reduced_accuracy_frame_at(obs_time);
	Apparent app = planetSource.apparent_in(frame);
	Horizontal hor = app.to_horizontal();

	double planetAltitude = hor.elevation().deg();
	double planetAzimuth = hor.azimuth().deg();

	cout << planetAzimuth << endl;
	cout << planetAltitude << endl;

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
	Camera camera(glm::vec3(0, 0, 5));

	renderer.Initialize(verticesData, sizeof(verticesData), indices, sizeof(indices));
	
	ShaderProgramSource source = ParseShader("OpenGLFiles/shaders/basic.shader");

	glm::mat4 trans;

	glm::vec3 position = glm::vec3(0, 0, 5);

	glm::mat4 projection = glm::perspective(glm::radians(camera.GetFoV()), float(screenWidth) / float(screenHeight), 0.1f, 100.0f); //creates a projection with a 45 degree FOV, the size of the window and clipping planes of 0.1 to 100


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
		camera.UpdateCamera(window.GetNativeWindow(), screenWidth, screenHeight);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

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
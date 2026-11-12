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

#include "OpenGLFiles/cpp/Shader.h"
#include "OpenGLFiles/cpp/Renderer.h"
#include "OpenGLFiles/cpp/Window.h"
#include "OpenGLFiles/cpp/Camera.h"

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

struct Object
{
	string objectName;
	glm::mat4 objectTransform;
	glm::vec4 objectColour;
};


const Planet getObjectName(const Object& object) {
	if (object.objectName == "Sun")
		return Planet::sun();
	if (object.objectName == "Mercury")
		return Planet::mercury();
	if (object.objectName == "Venus")
		return Planet::venus();
	if (object.objectName == "Moon")
		return Planet::moon();
	if (object.objectName == "Mars")
		return Planet::mars();
	if (object.objectName == "Jupiter")
		return Planet::jupiter();
	if (object.objectName == "Saturn")
		return Planet::saturn();
	if (object.objectName == "Uranus")
		return Planet::uranus();
	if (object.objectName == "Neptune")
		return Planet::neptune();
	if (object.objectName == "Pluto")
		return Planet::pluto();
	return Planet::earth(); // returns Earth as a fallback if somehow no object is found
}

vector<Object> renderObject;

double distanceRescale(double distance)
{
	distance = distance / 1000000;
	return distance;
}

int main() {

	t_calcephbin* ephemeris =
		calceph_open("OpenGLFiles/objectephemeris/de440s.bsp");

	calceph_prefetch(ephemeris);

	int status = novas_use_calceph_planets(ephemeris);

	//Declare all planet properties

	Object sun;
	sun.objectName = "Sun";
	sun.objectTransform = glm::mat4(1.0f); // add position
	sun.objectTransform = glm::translate(sun.objectTransform, glm::vec3(0, 0, 1));
	sun.objectColour = glm::vec4(1, 1, 0, 1);
	renderObject.push_back(sun);

	Object mercury;
	mercury.objectName = "Mercury";
	mercury.objectTransform = glm::mat4(1.0f);
	mercury.objectTransform = glm::translate(mercury.objectTransform, glm::vec3(0, 0, 1));
	mercury.objectTransform = glm::scale(mercury.objectTransform, glm::vec3(0.1f));
	mercury.objectColour = glm::vec4(0.1, 0.1, 0.1, 1);
	renderObject.push_back(mercury);

	Object venus;
	venus.objectName = "Venus";
	venus.objectTransform = glm::mat4(0.0f); // change, placeholder values
	venus.objectColour = glm::vec4(0, 0, 0, 0); // change, placeholder values
	renderObject.push_back(venus);

	Object moon;
	moon.objectName = "Moon";
	moon.objectTransform = glm::mat4(0.0f); // change, placeholder values
	moon.objectColour = glm::vec4(0, 0, 0, 0); // change, placeholder values
	renderObject.push_back(moon);

	Object mars;
	mars.objectName = "Mars";
	mars.objectTransform = glm::mat4(0.0f); // change, placeholder values
	mars.objectColour = glm::vec4(0, 0, 0, 0); // change, placeholder values
	renderObject.push_back(mars);


	Object jupiter;
	jupiter.objectName = "Jupiter";
	jupiter.objectTransform = glm::mat4(0.0f); // change, placeholder values
	jupiter.objectColour = glm::vec4(0, 0, 0, 0); // change, placeholder values
	renderObject.push_back(jupiter);

	Object saturn;
	saturn.objectName = "Saturn";
	saturn.objectTransform = glm::mat4(0.0f); // change, placeholder values
	saturn.objectColour = glm::vec4(0, 0, 0, 0); // change, placeholder values
	renderObject.push_back(saturn);

	Object uranus;
	uranus.objectName = "Uranus";
	uranus.objectTransform = glm::mat4(0.0f); // change, placeholder values
	uranus.objectColour = glm::vec4(0, 0, 0, 0); // change, placeholder values
	renderObject.push_back(uranus);

	Object neptune;
	neptune.objectName = "Neptune";
	neptune.objectTransform = glm::mat4(0.0f); // change, placeholder values
	neptune.objectColour = glm::vec4(0, 0, 0, 0); // change, placeholder values
	renderObject.push_back(neptune);

	Object pluto;
	pluto.objectName = "Pluto";
	pluto.objectTransform = glm::mat4(0.0f); // change, placeholder values
	pluto.objectColour = glm::vec4(0, 0, 0, 0); // change, placeholder values
	renderObject.push_back(pluto);

	for (Object& object : renderObject) {
		auto objectSource = getObjectName(object);
		Site site = Site::from_GPS(-1.087 * Unit::deg, 50.805 * Unit::deg, 60.0 * Unit::m);
		EOP eop(37, 0.6447 * Unit::s, 103.2 * Unit::mas, 211.3 * Unit::mas);
		auto obs = site.to_observer(eop);
		Time obs_time = Time::now(eop);
		Frame frame = obs.reduced_accuracy_frame_at(obs_time);
		Apparent app = objectSource.apparent_in(frame);
		Horizontal hor = app.to_horizontal();
		Coordinate objectDistance = app.distance();

		double objectAltitude = hor.elevation().deg();
		double objectAzimuth = hor.azimuth().deg();

		cout << object.objectName << endl;
		cout << objectAzimuth << endl;
		cout << objectAltitude	 << endl;
		cout << objectDistance.km() << " km" << endl;
		cout << distanceRescale(objectDistance.km()) << " units" << endl;
		cout << "\n";

		//z = std::sqrt((r ^ 2 * (cos(objectAzimuth)) ^ 2) / (1 + (cos(objectAzimuth)) ^ 2 * (tan(objectAltitude)) ^ 2));
		//x = z * tan(objectAzimuth);
		//y = z * tan(objectAltitude);
	}

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
	Camera camera(glm::vec3(0, 0, 5));

	renderer.Initialize(pointsVector.data(), pointsVector.size() * sizeof(glm::vec3), indicesVector.data(), indicesVector.size() * sizeof(unsigned int));
	
	ShaderProgramSource source = ParseShader("OpenGLFiles/shaders/basic.shader");

	glm::vec3 position = glm::vec3(0, 0, 5);

	glm::mat4 projection = glm::perspective(glm::radians(camera.GetFoV()), float(screenWidth) / float(screenHeight), 0.1f, 100.0f); //creates a projection with a 45 degree FOV, the size of the window and clipping planes of 0.1 to 100


	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader);

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


		for (Object& currentObject : renderObject)
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
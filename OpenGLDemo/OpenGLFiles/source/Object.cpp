#include "Object.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <novas-calceph.h>
#include <calceph.h>
#include <cmath>

using namespace supernovas;
using namespace std;

bool Object::initialiseEphemeris()
{
	ephemeris = calceph_open("OpenGLFiles/objectephemeris/de440s.bsp");

	if (ephemeris == nullptr)
	{
		return false;
	}

	calceph_prefetch(ephemeris);
	int status = novas_use_calceph_planets(ephemeris);

	return true;
}

Planet Object::getObjectName(const ObjectValues& objectValues) const{
	if (objectValues.objectName == "Sun")
		return Planet::sun();
	if (objectValues.objectName == "Mercury")
		return Planet::mercury();
	if (objectValues.objectName == "Venus")
		return Planet::venus();
	if (objectValues.objectName == "Moon")
		return Planet::moon();
	if (objectValues.objectName == "Mars")
		return Planet::mars();
	if (objectValues.objectName == "Jupiter")
		return Planet::jupiter();
	if (objectValues.objectName == "Saturn")
		return Planet::saturn();
	if (objectValues.objectName == "Uranus")
		return Planet::uranus();
	if (objectValues.objectName == "Neptune")
		return Planet::neptune();
	if (objectValues.objectName == "Pluto")
		return Planet::pluto();

	return Planet::earth(); // returns Earth as a fallback if somehow no object is found
}

double Object::distanceRescale(double distance)
{
	return distance / 1000000.0;
}

void Object::calculatePositions(std::vector <ObjectValues>& renderObject)
{
	for (ObjectValues& objectValues : renderObject) {
		auto objectSource = getObjectName(objectValues);
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

		cout << objectValues.objectName << endl;
		cout << objectAzimuth << endl;
		cout << objectAltitude << endl;
		cout << objectDistance.km() << " km" << endl;
		cout << distanceRescale(objectDistance.km()) << " units" << endl;
		cout << "\n";
	}
}

std::vector<ObjectValues> Object::createObjects()
{
	std::vector<ObjectValues> renderObject;

	ObjectValues sun;
	sun.objectName = "Sun";
	sun.objectTransform = glm::mat4(1.0f); // add position
	sun.objectTransform = glm::translate(sun.objectTransform, glm::vec3(0, 0, 1));
	sun.objectColour = glm::vec4(1, 1, 0, 1);
	renderObject.push_back(sun);

	ObjectValues mercury;
	mercury.objectName = "Mercury";
	mercury.objectTransform = glm::mat4(1.0f);
	mercury.objectTransform = glm::translate(mercury.objectTransform, glm::vec3(0, 0, 1));
	mercury.objectTransform = glm::scale(mercury.objectTransform, glm::vec3(0.1f));
	mercury.objectColour = glm::vec4(0.1, 0.1, 0.1, 1);
	renderObject.push_back(mercury);

	ObjectValues venus;
	venus.objectName = "Venus";
	venus.objectTransform = glm::mat4(0.0f); // change, placeholder values
	venus.objectColour = glm::vec4(0, 0, 0, 0); // change, placeholder values
	renderObject.push_back(venus);

	ObjectValues moon;
	moon.objectName = "Moon";
	moon.objectTransform = glm::mat4(0.0f); // change, placeholder values
	moon.objectColour = glm::vec4(0, 0, 0, 0); // change, placeholder values
	renderObject.push_back(moon);

	ObjectValues mars;
	mars.objectName = "Mars";
	mars.objectTransform = glm::mat4(0.0f); // change, placeholder values
	mars.objectColour = glm::vec4(0, 0, 0, 0); // change, placeholder values
	renderObject.push_back(mars);


	ObjectValues jupiter;
	jupiter.objectName = "Jupiter";
	jupiter.objectTransform = glm::mat4(0.0f); // change, placeholder values
	jupiter.objectColour = glm::vec4(0, 0, 0, 0); // change, placeholder values
	renderObject.push_back(jupiter);

	ObjectValues saturn;
	saturn.objectName = "Saturn";
	saturn.objectTransform = glm::mat4(0.0f); // change, placeholder values
	saturn.objectColour = glm::vec4(0, 0, 0, 0); // change, placeholder values
	renderObject.push_back(saturn);

	ObjectValues uranus;
	uranus.objectName = "Uranus";
	uranus.objectTransform = glm::mat4(0.0f); // change, placeholder values
	uranus.objectColour = glm::vec4(0, 0, 0, 0); // change, placeholder values
	renderObject.push_back(uranus);

	ObjectValues neptune;
	neptune.objectName = "Neptune";
	neptune.objectTransform = glm::mat4(0.0f); // change, placeholder values
	neptune.objectColour = glm::vec4(0, 0, 0, 0); // change, placeholder values
	renderObject.push_back(neptune);

	ObjectValues pluto;
	pluto.objectName = "Pluto";
	pluto.objectTransform = glm::mat4(0.0f); // change, placeholder values
	pluto.objectColour = glm::vec4(0, 0, 0, 0); // change, placeholder values
	renderObject.push_back(pluto);

	return renderObject;
}
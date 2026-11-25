#include "Object.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <novas.h>
#include <novas-calceph.h>
#include <calceph.h>
#include <cmath>
#include <algorithm>

using namespace supernovas;
using namespace std;

const double kmPerUnit = 1000000.0;
const double exaggerateScale = 200.0;

const double minRad = 1000;
const double maxRad = 70000;
const double minDeg = 0.1;
const double maxDeg = 0.4;

static double logScale(double radius, double distance)
{
	double logScale = log(radius / minRad) / log(maxRad / minRad);
	logScale = clamp(logScale, 0.0, 0.1);

	double angularDeg = minDeg + logScale * (maxDeg - minDeg);
	return distance * tan(glm::radians(angularDeg));
}

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
	return distance / kmPerUnit;
}

void Object::calculatePositions(std::vector <ObjectValues>& renderObject)
{
	Site site = Site::from_GPS(-1.087 * Unit::deg, 50.805 * Unit::deg, 60.0 * Unit::m);
	EOP eop(37, 0.6447 * Unit::s, 103.2 * Unit::mas, 211.3 * Unit::mas);
	auto obs = site.to_observer(eop);
	Time obs_time = Time::now(eop);
	Frame frame = obs.reduced_accuracy_frame_at(obs_time);

	for (ObjectValues& objectValues : renderObject) {
		auto objectSource = getObjectName(objectValues);
		Apparent app = objectSource.apparent_in(frame);
		Horizontal hor = app.to_horizontal();
		Coordinate objectDistance = app.distance();

		double objectAltitude = hor.elevation().deg();
		double objectAzimuth = hor.azimuth().deg();

		if (objectAzimuth < 0)
		{
			objectAzimuth += 360;
		}

		double altRad = glm::radians(objectAltitude);
		double azRad = glm::radians(objectAzimuth);

		float planetX, planetY, planetZ;

		planetX = (distanceRescale(objectDistance.km()) * cos(altRad)) * sin(azRad);
		planetY = (distanceRescale(objectDistance.km()) * cos(altRad)) * cos(azRad);
		planetZ = distanceRescale(objectDistance.km()) * sin(altRad);

		glm::vec3 position(planetX, planetZ, -planetY);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);

		double objRad;

		if (objectValues.exaggerateRadius)
			objRad = logScale(objectValues.objectRadius, distanceRescale(objectDistance.km()));
		else
			objRad = distanceRescale(objectValues.objectRadius);

		transform = glm::scale(transform, glm::vec3(objRad));
		objectValues.objectTransform = transform;

		cout << objectValues.objectName << ":" << endl;
		cout << "Azimuth: " << objectAzimuth << endl;
		cout << "Altitude: " << objectAltitude << endl;
		cout << "X: " << planetX << " " << "Y: " << planetY << " " << "Z: " << planetZ << "\n" << endl;

	}

	cout << obs_time.to_string() << endl;
}

std::vector<ObjectValues> Object::createObjects()
{
	std::vector<ObjectValues> renderObject;

	ObjectValues sun;
	sun.objectName = "Sun";
	sun.objectRadius = 695700.0;
	sun.exaggerateRadius = false;
	sun.objectColour = glm::vec4(1, 1, 0, 1);
	renderObject.push_back(sun);

	ObjectValues mercury;
	mercury.objectName = "Mercury";
	mercury.objectRadius = 2439.7;
	mercury.objectColour = glm::vec4(0.102, 0.102, 0.102, 1);
	renderObject.push_back(mercury);

	ObjectValues venus;
	venus.objectName = "Venus";
	venus.objectRadius = 6051.8;
	venus.objectColour = glm::vec4(0.972, 0.886, 0.690, 1); // change, placeholder values
	renderObject.push_back(venus);

	ObjectValues moon;
	moon.objectName = "Moon";
	moon.objectRadius = 1737.4;
	moon.exaggerateRadius = false;
	moon.objectColour = glm::vec4(0.608, 0.596, 0.580, 1); // change, placeholder values
	renderObject.push_back(moon);
		
	ObjectValues mars;
	mars.objectName = "Mars";
	mars.objectRadius = 3389.5;
	mars.objectColour = glm::vec4(0.600, 0.239, 0, 1); // change, placeholder values
	renderObject.push_back(mars);

	ObjectValues jupiter;
	jupiter.objectName = "Jupiter";
	jupiter.objectRadius = 69911.0;
	jupiter.objectColour = glm::vec4(0.690, 0.498, 0.208, 1); // change, placeholder values
	renderObject.push_back(jupiter);

	ObjectValues saturn;
	saturn.objectName = "Saturn";
	saturn.objectRadius = 58232.0;
	saturn.objectColour = glm::vec4(0.690, 0.561, 0.212, 1); // change, placeholder values
	renderObject.push_back(saturn);

	ObjectValues uranus;
	uranus.objectName = "Uranus";
	uranus.objectRadius = 25362.0;
	uranus.objectColour = glm::vec4(0.333, 0.502, 0.667, 1); // change, placeholder values
	renderObject.push_back(uranus);

	ObjectValues neptune;
	neptune.objectName = "Neptune";
	neptune.objectRadius = 24622.0;
	neptune.objectColour = glm::vec4(0.212, 0.408, 0.588, 1); // change, placeholder values
	renderObject.push_back(neptune);

	ObjectValues pluto;
	pluto.objectName = "Pluto";
	pluto.objectRadius = 1188.3;
	pluto.objectColour = glm::vec4(0.714, 0.616, 0.525, 1); // change, placeholder values
	renderObject.push_back(pluto);

	return renderObject;
}
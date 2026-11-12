#pragma once

#include <string>
#include <glm/glm.hpp>
#include <supernovas.h>
#include <vector>
#include <calceph.h>

struct ObjectValues
{
	std::string objectName;
	glm::mat4 objectTransform;
	glm::vec4 objectColour;
};

class Object
{
	public:
		bool initialiseEphemeris();
		std::vector<ObjectValues> createObjects();
		supernovas::Planet getObjectName(const ObjectValues& objectValues) const;
		double distanceRescale(double distance);
		void calculatePositions(std::vector<ObjectValues>& renderObject);

	private:
		t_calcephbin* ephemeris = nullptr;
};

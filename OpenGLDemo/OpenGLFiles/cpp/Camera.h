#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera
{
	private:
		glm::vec3 position; //cam position

		float horizontalAngle;
		float verticalAngle;
		float mouseSpeed;
		float FoV;

		glm::mat4 view;

	public:

		Camera(glm::vec3 startPosition);

		void UpdateCamera(GLFWwindow* window, int screenWidth, int screenHeight);

		float GetFoV() const;

		glm::mat4 GetViewMatrix();

};
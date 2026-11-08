#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

Camera::Camera(glm::vec3 startPosition)
{
	position = startPosition;

	horizontalAngle = 3.14f;
	verticalAngle = 0.0f;
	FoV = 45.0f;
	mouseSpeed = 0.005f;
	view = glm::mat4(1.0f);
}

void Camera::UpdateCamera(GLFWwindow* window, int screenWidth, int screenHeight)
{
	double mousePosX, mousePosY;

	glfwGetCursorPos(window, &mousePosX, &mousePosY);
	glfwSetCursorPos(window, screenWidth / 2, screenHeight / 2);
	horizontalAngle -= mouseSpeed * float(screenWidth / 2 - mousePosX);
	verticalAngle += mouseSpeed * float(screenHeight / 2 - mousePosY);

	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	glm::vec3 up = glm::cross(right, direction);

	view = glm::lookAt(
		position,
		position + direction,
		up
	);
}

glm::mat4 Camera::GetViewMatrix()
{
	return view;
}

float Camera::GetFoV() const
{
	return FoV;
}
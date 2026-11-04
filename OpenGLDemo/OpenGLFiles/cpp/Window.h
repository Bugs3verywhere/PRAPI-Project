#pragma once

#include <GLFW/glfw3.h>

class Window
{
	public:
		Window(int width, int height, const char* title);
		~Window();

		bool ShouldClose();
		void SwapBuffers();
		void PollEvents();

		GLFWwindow* GetNativeWindow();
private:
	GLFWwindow* window = nullptr;

};
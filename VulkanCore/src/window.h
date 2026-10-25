#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace Engine {

	class EngineWindow {
	public:
		EngineWindow(int w, int h, std::string name);
		~EngineWindow();

		EngineWindow(const EngineWindow&) = delete;
		EngineWindow& operator=(const EngineWindow&) = delete;

		bool shouldClose() { return glfwWindowShouldClose(window); }

		GLFWwindow* GetWindow() { return window; }

	private:
		void initWindow();
		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

		const int width;
		const int height;

		std::string windowName;
		GLFWwindow* window;
	};
}
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Engine {

	class EngineSurface {
	public:
		EngineSurface() {};
		void createSurface(VkInstance instance, GLFWwindow* window);

		void destroySurface(VkInstance instance);
		
		VkSurfaceKHR GetSurface() { return surface; }

	private:
		VkSurfaceKHR surface;
	};
}
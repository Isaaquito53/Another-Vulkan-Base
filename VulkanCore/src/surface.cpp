#include "surface.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

namespace Engine {

	void EngineSurface::createSurface(VkInstance instance, GLFWwindow* window)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
	}

	void EngineSurface::destroySurface(VkInstance instance)
	{
		vkDestroySurfaceKHR(instance, surface, nullptr);
	}
}
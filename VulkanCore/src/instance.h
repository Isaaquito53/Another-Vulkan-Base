#pragma once

#include "debugMessenger.h"
#include "device.h"
#include "surface.h"
#include "swapchain.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

namespace Engine {

	class EngineInstance {
	public:
		EngineInstance(GLFWwindow* window);
		~EngineInstance();

		EngineDevice GetEngineDevice() { return engineDevice; }
		EngineSurface GetEngineSurface() { return engineSurface; }
		EngineSwapchain& GetEngineSwapchain() { return engineSwapchain; }
		VkInstance GetInstance() { return instance; }

	private:
		void createInstance();
		std::vector<const char*> getRequiredExtensions();

		VkInstance instance;
		EngineDebugMessenger engineDebugMessenger;
		EngineDevice engineDevice;
		EngineSurface engineSurface;
		EngineSwapchain engineSwapchain;
	};
}
#pragma once

#include "queueFamilyIndices.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <optional>
#include <vector>

namespace Engine {

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	class EngineDevice {
	public:
		EngineDevice() {};

		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
		void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
		void createLogicalDevice(VkSurfaceKHR surface);
		void destroyDevice();

		VkPhysicalDevice GetPhysicalDevice() { return physicalDevice; }
		VkDevice GetDevice() { return device; }
		VkQueue GetGraphicsQueue() { return graphicsQueue; }
		VkQueue GetPresentQueue() { return presentQueue; }

	private:
		bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice device;
		VkQueue graphicsQueue;
		VkQueue presentQueue;
	};
}
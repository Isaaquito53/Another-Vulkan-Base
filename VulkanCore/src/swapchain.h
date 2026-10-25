#pragma once

#include "queueFamilyIndices.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

namespace Engine {

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class EngineSwapchain {
	public:
		EngineSwapchain() {}
		void createSwapChain(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, GLFWwindow* window, QueueFamilyIndices queueIndices, VkDevice device);
		void createImageViews(VkDevice device);

		void destroySwapchain(VkDevice device);
		void destroyImageViews(VkDevice device);

		VkSwapchainKHR GetSwapchain() { return swapChain; }
		VkFormat GetFormat() { return swapChainImageFormat; }
		VkExtent2D GetExtent() { return swapChainExtent; }
		std::vector<VkImageView> GetImageViews() { return swapChainImageViews; }

	private:
		VkSwapchainKHR swapChain;
		std::vector<VkImage> swapChainImages;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		std::vector<VkImageView> swapChainImageViews;
	};

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
}
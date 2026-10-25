#include "instance.h"
#include "validationLayers.h"

#include <stdexcept>

namespace Engine {

	EngineInstance::EngineInstance(GLFWwindow* window)
	{
		createInstance();
		engineDebugMessenger.setupDebugMessenger(instance);
		engineSurface.createSurface(instance, window);
		engineDevice.pickPhysicalDevice(instance, engineSurface.GetSurface());
		engineDevice.createLogicalDevice(engineSurface.GetSurface());
		engineSwapchain.createSwapChain(engineDevice.GetPhysicalDevice(), engineSurface.GetSurface(), window,
			engineDevice.findQueueFamilies(engineDevice.GetPhysicalDevice(), engineSurface.GetSurface()),
			engineDevice.GetDevice());
		engineSwapchain.createImageViews(engineDevice.GetDevice());
	}

	EngineInstance::~EngineInstance()
	{
		engineDevice.destroyDevice();
		if (enableValidationLayers)
		{
			engineDebugMessenger.DestroyDebugUtilsMessengerEXT(instance, nullptr);
		}

		engineSurface.destroySurface(instance);
		vkDestroyInstance(instance, nullptr);
	}

	void EngineInstance::createInstance()
	{
		// Checks if all of the requested layers are available
		if (enableValidationLayers && !checkValidationLayerSupport()) {
			throw std::runtime_error("validation layers requested, but not available!");
		}

		// Useful info for driver optimizations
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		// Choose extensions and validation layers
		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		// Return the required list of extensions based on whether validation layers are enabled or not
		auto extensions = getRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		// Include the validation layer names if they are enabled
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();

			engineDebugMessenger.populateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else {
			createInfo.enabledLayerCount = 0;

			createInfo.pNext = nullptr;
		}

		// Check if instance handle was created successfully
		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
			throw std::runtime_error("failed to create instance!");
		}
	}

	std::vector<const char*> EngineInstance::getRequiredExtensions() {
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}
}
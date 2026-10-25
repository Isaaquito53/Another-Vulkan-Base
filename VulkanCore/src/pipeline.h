#pragma once

#include "queueFamilyIndices.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <windows.h>
#include <commdlg.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <string>
#include <vector>

#include "Scene.h"

namespace Engine {

	const int MAX_FRAMES_IN_FLIGHT = 2;
	const int MAX_INSTANCES = 1024;

	class EnginePipeline {
	public:

		int m_showImGUI = 0;
		bool m_loadShaders = false;
		bool m_saveShaders = false;
		char m_fragmentLoaded[50] = "";
		char m_fragmentSaved[50] = "";

		EnginePipeline(const std::string& vertFilepath, const std::string& fragFilepath, VkFormat swapChainImageFormat, 
			VkDevice device, std::vector<VkImageView> swapChainImageViews, VkExtent2D swapChainExtent,
			QueueFamilyIndices queueIndices, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue);

		void createFramebuffers(VkDevice device, std::vector<VkImageView> swapChainImageViews, VkExtent2D swapChainExtent);

		void createDepthResources(VkExtent2D swapChainExtent, VkDevice device, VkPhysicalDevice physicalDevice);

		void destroyShaders(VkDevice device);
		void destroyPipeline(VkDevice device);
		void destroySwapChainFramebuffers(VkDevice device);

		void recordCommandBuffer(unsigned int cmdBufferIndex, uint32_t imageIndex, 
			VkExtent2D swapChainExtent, uint32_t crrntFrame, VkDevice device, float engine_time);

		void updateUniformBuffer(uint32_t currentImage, VkExtent2D swapChainExtent, float time, double xpos, double ypos);

		VkCommandBuffer beginSingleTimeCommands(VkDevice device);
		void endSingleTimeCommands(VkDevice device, VkQueue graphicsQueue, VkCommandBuffer commandBuffer);

		const VkFence* GetFence(unsigned int fenceIndex) { return &inFlightFences[fenceIndex]; }
		VkSemaphore GetImageAvailableSemaphore(unsigned int semIndex) { return imageAvailableSemaphores[semIndex]; }
		VkSemaphore GetRenderFinishedSemaphore(unsigned int semIndex) { return renderFinishedSemaphores[semIndex]; }
		VkCommandBuffer GetCommandBuffer(unsigned int cmdBufferIndex) { return commandBuffers[cmdBufferIndex]; }
		const VkCommandBuffer* GetPCommandBuffer(unsigned int cmdBufferIndex) { return &commandBuffers[cmdBufferIndex]; }
		VkDescriptorPool GetDescriptorPool() { return descriptorPool; }
		VkRenderPass GetRenderPass() { return renderPass; }

		void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, VkDevice device);

		void updateCam(float time);
		void selectInstance(VkDevice device, int w, int h);
		void showInstanceMenu(VkDevice device, unsigned int cmdBufferIndex);

	private:
		static std::vector<char> readFile(const std::string& filepath);

		VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice device);
		void createRenderPass(VkFormat swapChainImageFormat, VkDevice device, VkPhysicalDevice physicalDevice);
		void createCommandPool(VkDevice device, QueueFamilyIndices queueIndices);
		void createCommandBuffers(VkDevice device);

		void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, 
			VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkDevice device, VkPhysicalDevice physicalDevice);
		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDevice device, VkQueue graphicsQueue);
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice);
		// Vertex 
		void createVertexBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue);
		// Instance
		void createInstanceBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue);
		void updateInstanceBuffer(VkDevice device, int instanceID, InstanceParams param);
		// Index
		void createIndexBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue);

		void createDescriptorSetLayout(VkDevice device);
		void createUniformBuffers(VkDevice device, VkPhysicalDevice physicalDevice);
		void createDescriptorPool(VkDevice device);
		void createDescriptorSets(VkDevice device);

		void transitionImageLayout(VkDevice device, VkQueue graphicsQueue, 
			VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		void copyBufferToImage(VkDevice device, VkQueue graphicsQueue, VkBuffer buffer, VkImage image, 
			uint32_t width, uint32_t height);
		void createImage(VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width, uint32_t height, 
			VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, 
			VkImage& image, VkDeviceMemory& imageMemory);
		void createTextureImage(VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue);

		VkImageView createImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
		void createTextureImageView(VkDevice device);
		void createTextureSampler(VkDevice device, VkPhysicalDevice physicalDevice);

		VkFormat findSupportedFormat(VkPhysicalDevice physicalDevice, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		VkFormat findDepthFormat(VkPhysicalDevice physicalDevice);
		bool hasStencilComponent(VkFormat format);

		void loadModel();

		void createSyncObjects(VkDevice device);

		VkRenderPass renderPass;
		VkPipelineLayout pipelineLayout;
		VkPipeline graphicsPipeline;
		std::vector<VkFramebuffer> swapChainFramebuffers;

		VkCommandPool commandPool;
		std::vector<VkCommandBuffer> commandBuffers;

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;

		VkDescriptorSetLayout descriptorSetLayout;
		VkDescriptorPool descriptorPool;
		std::vector<VkDescriptorSet> descriptorSets;

		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;

		VkBuffer instanceBuffer;
		VkDeviceMemory instanceBufferMemory;

		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;

		std::vector<VkBuffer> uniformBuffers;
		std::vector<VkDeviceMemory> uniformBuffersMemory;
		std::vector<void*> uniformBuffersMapped;

		VkImage textureImage;
		VkDeviceMemory textureImageMemory;
		VkImageView textureImageView;
		VkSampler textureSampler;

		VkImage depthImage;
		VkDeviceMemory depthImageMemory;
		VkImageView depthImageView;

		void showImGuiOptions(VkDevice device, unsigned int cmdBufferIndex, float engine_time);

		UniformBufferObject ubo;

		Scene m_scene;

		glm::vec3 m_translation = glm::vec3(0.0);
		glm::vec3 m_rotation = glm::vec3(1.0);
		float m_rotationAlpha = 0.0;
		glm::vec3 m_scale = glm::vec3(1.0);

		float m_time = 0.0;

		void Tree(VkDevice device, int maxDepth);
		void RecursiveTree(std::vector<Instance>& parts, int maxDepth, int depth, Instance parent);

		int m_maxDepth = 0;
		float m_angL = 25;
		float m_angR = -35;
	};
}
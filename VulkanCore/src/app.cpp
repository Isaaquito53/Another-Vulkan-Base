#include "app.h"

#include <fstream>
#include <stdexcept>
#include <iostream>

#include "imgui/imconfig.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_vulkan.h"

namespace Engine {
	// Create global variables to control the frag shader
	float engine_time = glfwGetTime();
	double xpos = 0., ypos = 0.;

	void App::run()
	{
		initImgui();
		ImGui::SetNextWindowSize(ImVec2(300, 300));

		while (!engineWindow.shouldClose())
		{
			glfwPollEvents();

			drawFrame();

			if (enginePipeline.m_loadShaders)
			{
				std::string fragment = std::string(enginePipeline.m_fragmentLoaded);
				const std::string filepath = "NoVulkan/res/shaders/" + fragment + ".frag";
				std::ifstream file(filepath, std::ios::ate | std::ios::binary);

				if (file.is_open())
				{
					recreateSwapChain();
					enginePipeline.destroyShaders(engineInstance.GetEngineDevice().GetDevice());
					std::string sCall = "C:/VulkanSDK/1.4.304.1/Bin/glslc.exe NoVulkan/res/shaders/" + fragment + ".frag -o NoVulkan/res/shaders/" + fragment + ".frag.spv";
					const char* cCall = sCall.c_str();
					system(cCall);
					enginePipeline.createGraphicsPipeline("NoVulkan/res/shaders/simple_shader.vert.spv", filepath+".spv",
						engineInstance.GetEngineDevice().GetDevice());
				}
				enginePipeline.m_loadShaders = false;
			}
			if (enginePipeline.m_saveShaders)
			{
				std::string loadFragment = std::string(enginePipeline.m_fragmentLoaded);
				std::string saveFragment = std::string(enginePipeline.m_fragmentSaved);
				std::string sCall = "copy \".\\shaders\\" + loadFragment + ".frag\" \".\\shaders\\" + saveFragment + ".frag\"";
				const char* cCall = sCall.c_str();
				system(cCall);
				enginePipeline.m_saveShaders = false;
			}
		}

		vkDeviceWaitIdle(engineInstance.GetEngineDevice().GetDevice());
	}

	void App::initImgui()
	{
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForVulkan(engineWindow.GetWindow(), true);

		ImGui_ImplVulkan_InitInfo info = {};
		info.Instance = engineInstance.GetInstance();
		info.PhysicalDevice = engineInstance.GetEngineDevice().GetPhysicalDevice();
		info.Device = engineInstance.GetEngineDevice().GetDevice();
		info.DescriptorPool = enginePipeline.GetDescriptorPool();
		info.RenderPass = enginePipeline.GetRenderPass();
		info.Queue = engineInstance.GetEngineDevice().GetGraphicsQueue();
		info.MinImageCount = MAX_FRAMES_IN_FLIGHT;
		info.ImageCount = MAX_FRAMES_IN_FLIGHT;
		info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		ImGui_ImplVulkan_Init(&info);

		VkCommandBuffer commandBuffer = beginSingleTimeCommands(engineInstance.GetEngineDevice().GetDevice(), enginePipeline.GetCommandPool());
		endSingleTimeCommands(engineInstance.GetEngineDevice().GetDevice(), enginePipeline.GetCommandPool(), engineInstance.GetEngineDevice().GetGraphicsQueue(), commandBuffer);

		vkDeviceWaitIdle(engineInstance.GetEngineDevice().GetDevice());
	}

	void App::drawFrame()
	{
		// device = engineInstance.GetEngineDevice().GetDevice()
		// inFlightFence = enginePipeline.GetFence()
		// swapChain = engineInstance.GetEngineSwapchain().GetSwapchain()
		// imageAvailableSemaphore = enginePipeline.GetImageAvailableSemaphore()
		// commandBuffer = enginePipeline.GetCommandBuffer()
		// recordCommandBuffer(commandBuffer, imageIndex) = enginePipeline.recordCommandBuffer(imageIndex)
		// renderFinishedSemaphore = enginePipeline.GetRenderFinishedSemaphore()
		// graphicsQueue = engineInstance.GetEngineDevice().GetGraphicsQueue()
		// presentQueue = engineInstance.GetEngineDevice().GetPresentQueue()

		// Waiting for the previous frame
		vkWaitForFences(engineInstance.GetEngineDevice().GetDevice(), 1, enginePipeline.GetFence(crrntFrame), VK_TRUE, UINT64_MAX);

		// Acquiring an image from the swap chain
		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(engineInstance.GetEngineDevice().GetDevice(), engineInstance.GetEngineSwapchain().GetSwapchain(),
			UINT64_MAX, enginePipeline.GetImageAvailableSemaphore(crrntFrame), VK_NULL_HANDLE, &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		// Get time
		engine_time = glfwGetTime();
		
		// Get mouse input
		int state = glfwGetMouseButton(engineWindow.GetWindow(), GLFW_MOUSE_BUTTON_LEFT);
		if (state == GLFW_PRESS)
			glfwGetCursorPos(engineWindow.GetWindow(), &xpos, &ypos);

		// Get tab key released to enable/disable the imgui window
		int tabState = glfwGetKey(engineWindow.GetWindow(), GLFW_KEY_TAB);
		if (tabState == GLFW_PRESS && enginePipeline.m_showImGUI == 0)
			enginePipeline.m_showImGUI = 1;
		if (tabState == GLFW_RELEASE && enginePipeline.m_showImGUI == 1)
			enginePipeline.m_showImGUI = 2;
		if (tabState == GLFW_PRESS && enginePipeline.m_showImGUI == 2)
			enginePipeline.m_showImGUI = 3;
		if (tabState == GLFW_RELEASE && enginePipeline.m_showImGUI == 3)
			enginePipeline.m_showImGUI = 0;

		//std::cout << time << std::endl;
		enginePipeline.updateUniformBuffer(crrntFrame, engineInstance.GetEngineSwapchain().GetExtent(), engine_time, xpos, ypos);
		enginePipeline.GetSceneTranslator().updateCam(engine_time);
		enginePipeline.GetSceneTranslator().selectInstance(engineInstance.GetEngineDevice().GetDevice(), enginePipeline.GetUBO(), WIDTH, HEIGHT);
		enginePipeline.recordCommandBuffer(crrntFrame, imageIndex, engineInstance.GetEngineSwapchain().GetExtent(), crrntFrame, engineInstance.GetEngineDevice().GetDevice(), engine_time);

		vkResetFences(engineInstance.GetEngineDevice().GetDevice(), 1, enginePipeline.GetFence(crrntFrame));

		// Recording the command buffer
		//vkResetCommandBuffer(enginePipeline.GetCommandBuffer(crrntFrame), /*VkCommandBufferResetFlagBits*/ 0);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		// Submitting the command buffer
		VkSemaphore waitSemaphores[] = { enginePipeline.GetImageAvailableSemaphore(crrntFrame) };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = enginePipeline.GetPCommandBuffer(crrntFrame);

		VkSemaphore signalSemaphores[] = { enginePipeline.GetRenderFinishedSemaphore(crrntFrame) };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(engineInstance.GetEngineDevice().GetGraphicsQueue(), 1, &submitInfo, *enginePipeline.GetFence(crrntFrame)) != VK_SUCCESS) {
			throw std::runtime_error("failed to submit draw command buffer!");
		}

		// Presentation
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { engineInstance.GetEngineSwapchain().GetSwapchain() };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;

		presentInfo.pImageIndices = &imageIndex;

		result = vkQueuePresentKHR(engineInstance.GetEngineDevice().GetPresentQueue(), &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
			framebufferResized = false;
			recreateSwapChain();
		}
		else if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image!");
		}

		crrntFrame = (crrntFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void App::recreateSwapChain()
	{
		int width = 0, height = 0;
		glfwGetFramebufferSize(engineWindow.GetWindow(), &width, &height);
		while (width == 0 || height == 0) {
			glfwGetFramebufferSize(engineWindow.GetWindow(), &width, &height);
			glfwWaitEvents();
		}

		WIDTH = width;
		HEIGHT = height;

		vkDeviceWaitIdle(engineInstance.GetEngineDevice().GetDevice());

		// Clean Swapchain
		enginePipeline.destroySwapChainFramebuffers(engineInstance.GetEngineDevice().GetDevice());
		engineInstance.GetEngineSwapchain().destroySwapchain(engineInstance.GetEngineDevice().GetDevice());

		// Create Swapchain
		engineInstance.GetEngineSwapchain().createSwapChain(engineInstance.GetEngineDevice().GetPhysicalDevice(),
			engineInstance.GetEngineSurface().GetSurface(), engineWindow.GetWindow(), 
			engineInstance.GetEngineDevice().findQueueFamilies(engineInstance.GetEngineDevice().GetPhysicalDevice(), 
			engineInstance.GetEngineSurface().GetSurface()), 
			engineInstance.GetEngineDevice().GetDevice());

		// Create ImageViews
		engineInstance.GetEngineSwapchain().createImageViews(engineInstance.GetEngineDevice().GetDevice());

		// Create Depth resources
		enginePipeline.createDepthResources(engineInstance.GetEngineSwapchain().GetExtent(),
			engineInstance.GetEngineDevice().GetDevice(), engineInstance.GetEngineDevice().GetPhysicalDevice());

		// Create Framebuffers
		enginePipeline.createFramebuffers(engineInstance.GetEngineDevice().GetDevice(),
			engineInstance.GetEngineSwapchain().GetImageViews(), engineInstance.GetEngineSwapchain().GetExtent());
	}

	App::~App()
	{
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		enginePipeline.destroySwapChainFramebuffers(engineInstance.GetEngineDevice().GetDevice());
		engineInstance.GetEngineSwapchain().destroySwapchain(engineInstance.GetEngineDevice().GetDevice());
		enginePipeline.destroyPipeline(engineInstance.GetEngineDevice().GetDevice());
	}
}
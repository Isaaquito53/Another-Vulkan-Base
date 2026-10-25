#pragma once

#include "instance.h"
#include "window.h"
#include "pipeline.h"

namespace Engine {

	class App {
	public:

		int WIDTH = 600;
		int HEIGHT = 600;

		void run();
		~App();

		bool framebufferResized = false;

	private:
		void drawFrame();
		void recreateSwapChain();
		void initImgui();

		EngineWindow engineWindow{ WIDTH, HEIGHT, "Vulkan app" };
		EngineInstance engineInstance{engineWindow.GetWindow()};
		EnginePipeline enginePipeline{ "NoVulkan/res/shaders/simple.vert.spv", "NoVulkan/res/shaders/simple.frag.spv", 
			engineInstance.GetEngineSwapchain().GetFormat(), engineInstance.GetEngineDevice().GetDevice(), 
			engineInstance.GetEngineSwapchain().GetImageViews(), engineInstance.GetEngineSwapchain().GetExtent(),
			engineInstance.GetEngineDevice().findQueueFamilies(engineInstance.GetEngineDevice().GetPhysicalDevice(), 
			engineInstance.GetEngineSurface().GetSurface()), engineInstance.GetEngineDevice().GetPhysicalDevice(), 
			engineInstance.GetEngineDevice().GetGraphicsQueue()};
		
		uint32_t crrntFrame = 0;
	};
}
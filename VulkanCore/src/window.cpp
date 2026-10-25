#include "window.h"
#include "app.h"

namespace Engine {
	EngineWindow::EngineWindow(int w, int h, std::string name):
		width{w}, height{h}, windowName{name}
	{
		initWindow();
	}

	EngineWindow::~EngineWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void EngineWindow::initWindow()
	{
		// Init Window and disable OpenGL context
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		// When resizing the window, tell to the App instance to change the swapchain
		//glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	}

	void EngineWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
		app->framebufferResized = true;
	}
}
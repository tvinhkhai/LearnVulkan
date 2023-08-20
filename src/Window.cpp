#include "stdafx.h"
#include "Window.h"

#include "VulkanAPI/RequiredInstanceExtensionsInfo.h"

#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_NONE
#define VK_USE_PLATFORM_WIN32_KHR
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

///////////////////////////////////////////////////////////////////////////////
namespace
{
	constexpr uint32_t k_WIDTH = 800;
	constexpr uint32_t k_HEIGHT = 600;
}
///////////////////////////////////////////////////////////////////////////////

Window::Window()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // to not create opengl context
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_window = glfwCreateWindow(k_WIDTH, k_WIDTH, "Vulkan", nullptr, nullptr);
}

///////////////////////////////////////////////////////////////////////////////

Window::~Window()
{
	glfwDestroyWindow(m_window);
	
	glfwTerminate();
}

///////////////////////////////////////////////////////////////////////////////

bool Window::IsExiting()
{
	return glfwWindowShouldClose(m_window);
}

///////////////////////////////////////////////////////////////////////////////

void Window::Update()
{
	glfwPollEvents();
}

///////////////////////////////////////////////////////////////////////////////

VulkanAPI::RequiredInstanceExtensionsInfo Window::GetRequiredInstanceExtensionsInfo()
{
	VulkanAPI::RequiredInstanceExtensionsInfo info;
	info.Extensions = glfwGetRequiredInstanceExtensions(&info.Count);

	return info;
}

///////////////////////////////////////////////////////////////////////////////

VkSurfaceKHR Window::CreateVulkanSurface(VkInstance i_instance)
{
	VkSurfaceKHR surface;
	if (glfwCreateWindowSurface(i_instance, m_window, nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}

	return surface;
}

///////////////////////////////////////////////////////////////////////////////

Window::Size Window::GetFramebufferSize()
{
	Window::Size size;

	glfwGetFramebufferSize(m_window, &size.width, &size.height);

	return size;
}

///////////////////////////////////////////////////////////////////////////////
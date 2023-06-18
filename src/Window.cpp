#include "stdafx.h"
#include "Window.h"

#include "VulkanAPI/RequiredInstanceExtensionsInfo.h"

#define GLFW_INCLUDE_NONE
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

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
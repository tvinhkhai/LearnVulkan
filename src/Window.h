#pragma once

struct GLFWwindow;

namespace VulkanAPI
{
struct RequiredInstanceExtensionsInfo;
}

class Window {
///////////////////////////////////////////////////////////////////////////////
public:
    Window();
    ~Window();

    bool IsExiting();
    void Update();

    VulkanAPI::RequiredInstanceExtensionsInfo GetRequiredInstanceExtensionsInfo();
    VkSurfaceKHR CreateVulkanSurface(VkInstance i_instance);

private:
    GLFWwindow* m_window;

///////////////////////////////////////////////////////////////////////////////
};
#pragma once

struct GLFWwindow;

namespace VulkanAPI
{
struct RequiredInstanceExtensionsInfo;
}

class Window {
///////////////////////////////////////////////////////////////////////////////
public:

    struct Size
    {
        int width;
        int height;
    };

    Window();
    ~Window();

    bool IsExiting();
    void Update();

    VulkanAPI::RequiredInstanceExtensionsInfo GetRequiredInstanceExtensionsInfo();
    VkSurfaceKHR CreateVulkanSurface(VkInstance i_instance);

    Size GetFramebufferSize();

private:
    GLFWwindow* m_window;

///////////////////////////////////////////////////////////////////////////////
};
#pragma once

#include "VulkanAPI/VulkanAPI_fwd.inl"

namespace VulkanAPI
{
///////////////////////////////////////////////////////////////////////////////
class WindowSurface {
///////////////////////////////////////////////////////////////////////////////
public:
    WindowSurface(VkInstance i_instance, VkSurfaceKHR i_surface);
    ~WindowSurface();

    VkSurfaceKHR GetSurface();

private:
    VkInstance m_instance;
    VkSurfaceKHR m_surface;
};
///////////////////////////////////////////////////////////////////////////////
} //namespace Instance
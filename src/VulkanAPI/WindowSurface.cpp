#include "stdafx.h"
#include "WindowSurface.h"

#include <vulkan/vulkan.h>

namespace VulkanAPI
{
///////////////////////////////////////////////////////////////////////////////

WindowSurface::WindowSurface(VkInstance i_instance, VkSurfaceKHR i_surface)
    : m_instance(i_instance)
    ,m_surface(i_surface)
{

}

///////////////////////////////////////////////////////////////////////////////

WindowSurface::~WindowSurface()
{
    vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
}

///////////////////////////////////////////////////////////////////////////////

VkSurfaceKHR WindowSurface::GetSurface()
{
    return m_surface;
}

///////////////////////////////////////////////////////////////////////////////
} //namespace WindowSurface
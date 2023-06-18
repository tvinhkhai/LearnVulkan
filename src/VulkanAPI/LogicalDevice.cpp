#include "stdafx.h"
#include "LogicalDevice.h"

#include "VulkanAPI/QueueFamilyIndices.h"

#include <vulkan/vulkan.h>

namespace VulkanAPI
{
///////////////////////////////////////////////////////////////////////////////

LogicalDevice::LogicalDevice(VkDevice i_device, QueueFamilyIndices i_queueFamilyIndices)
    : m_device(i_device)
{
    vkGetDeviceQueue(m_device, i_queueFamilyIndices.optGraphicsFamily.value(), 0, &m_graphicsQueue);
}

///////////////////////////////////////////////////////////////////////////////

LogicalDevice::~LogicalDevice()
{
    vkDestroyDevice(m_device, nullptr);
}

///////////////////////////////////////////////////////////////////////////////
} //namespace LogicalDevice
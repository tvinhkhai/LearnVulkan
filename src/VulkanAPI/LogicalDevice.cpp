#include "stdafx.h"
#include "LogicalDevice.h"

#include "VulkanAPI/QueueFamilyIndices.h"

#include <vulkan/vulkan.h>

namespace VulkanAPI
{
///////////////////////////////////////////////////////////////////////////////

LogicalDevice::LogicalDevice(VkDevice i_device, QueueFamilyIndices i_queueFamilyIndices)
    : m_device(i_device)
    , m_graphicsQueue(nullptr)
    , m_presentQueue(nullptr)
{
    vkGetDeviceQueue(m_device, i_queueFamilyIndices.optGraphicsFamily.value(), 0, &m_graphicsQueue);
    vkGetDeviceQueue(m_device, i_queueFamilyIndices.optPresentFamily.value(), 0, &m_presentQueue);
}

///////////////////////////////////////////////////////////////////////////////

LogicalDevice::~LogicalDevice()
{
    vkDestroyDevice(m_device, nullptr);
}

///////////////////////////////////////////////////////////////////////////////
} //namespace LogicalDevice
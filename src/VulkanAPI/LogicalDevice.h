#pragma once

#include "VulkanAPI/VulkanAPI_fwd.inl"

namespace VulkanAPI
{
    struct QueueFamilyIndices;
}

namespace VulkanAPI
{
///////////////////////////////////////////////////////////////////////////////
class LogicalDevice {
///////////////////////////////////////////////////////////////////////////////
public:
    LogicalDevice(VkDevice i_device, QueueFamilyIndices i_queueFamilyIndices);
    ~LogicalDevice();

private:
    VkDevice m_device;
    VkQueue m_graphicsQueue;
};
///////////////////////////////////////////////////////////////////////////////
} //namespace Instance
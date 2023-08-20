#pragma once

#include "VulkanAPI/QueueFamilyIndices.h"

namespace VulkanAPI
{
    class LogicalDevice;
}

namespace VulkanAPI
{
///////////////////////////////////////////////////////////////////////////////
class PhysicalDevice {
///////////////////////////////////////////////////////////////////////////////
public:
    PhysicalDevice(VkPhysicalDevice i_device, QueueFamilyIndices& i_queueFamilyIndices);
    ~PhysicalDevice();

    void CreateLogicalDevice(const std::vector<const char*>& i_validationLayers, const std::vector<const char*>& i_deviceExtensions);
    VkPhysicalDevice GetDevice()
    {
        return m_device;
    }

    QueueFamilyIndices GetQueueFamilyIndices()
    {
        return m_queueFamilyIndices;
    }

    LogicalDevice* GetLogicalDevice()
    {
        return m_logicalDevice.get();
    }

private:
    VkPhysicalDevice m_device;
    QueueFamilyIndices m_queueFamilyIndices;
    std::unique_ptr<LogicalDevice> m_logicalDevice;
};
///////////////////////////////////////////////////////////////////////////////
} //namespace Instance
#pragma once

#include "VulkanAPI/VulkanAPI_fwd.inl"
#include "VulkanAPI/QueueFamilyIndices.h"

namespace VulkanAPI
{
    //struct QueueFamilyIndices;
    class LogicalDevice;
}

namespace VulkanAPI
{
///////////////////////////////////////////////////////////////////////////////
class PhysicalDevice {
///////////////////////////////////////////////////////////////////////////////
public:
    PhysicalDevice(VkPhysicalDevice i_device);
    ~PhysicalDevice();

    void CreateLogicalDevice(const std::vector<const char*>& i_validationLayers, QueueFamilyIndices& i_queueFamilyIndices);

private:
    VkPhysicalDevice m_device;
    QueueFamilyIndices m_queueFamilyIndices;
    std::unique_ptr<LogicalDevice> m_logicalDevice;
};
///////////////////////////////////////////////////////////////////////////////
} //namespace Instance
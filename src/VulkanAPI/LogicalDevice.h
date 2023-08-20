#pragma once

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

    VkDevice GetDevice()
    {
        return m_device;
    }

    VkQueue GetGraphicsQueue()
    {
        return m_graphicsQueue;
    }

    VkQueue GetPresentQueue()
    {
        return m_presentQueue;
    }

private:
    VkDevice m_device;
    VkQueue m_graphicsQueue;
    VkQueue m_presentQueue;
};
///////////////////////////////////////////////////////////////////////////////
} //namespace Instance
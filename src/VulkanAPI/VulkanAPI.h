#pragma once

#include "VulkanAPI/VulkanAPI_fwd.inl"

class Window;

namespace VulkanAPI
{
class Instance;
struct QueueFamilyIndices;
}

namespace VulkanAPI
{
///////////////////////////////////////////////////////////////////////////////
class VulkanAPI {
///////////////////////////////////////////////////////////////////////////////
public:
    VulkanAPI();
    ~VulkanAPI();
    void CreateInstance(std::unique_ptr<Window>& i_window, bool i_enableValidationLayers);
    void SetupDebugMessenger();
    void PickPhysicalDevice();
    void CreateLogicalDevice();

#if defined(DEBUG)
    void PrintAvailableExtensions();
#endif

private:
    bool CheckValidationLayerSupport();
    std::vector<const char*> GetRequiredExtensions(std::unique_ptr<Window>& i_window);

    int RateDeviceSuitability(VkPhysicalDevice device);
    bool IsDeviceSuitable(VkPhysicalDevice device);

    QueueFamilyIndices FindQueueFamily(VkPhysicalDevice device);

private:
    std::unique_ptr<Instance> m_instance;
    bool m_enableValidationLayers;
    VkDebugUtilsMessengerEXT m_debugMessenger;
    VkPhysicalDevice m_physicalDevice;
    VkDevice m_logicalDevice;
    VkQueue m_graphicsQueue;
};
///////////////////////////////////////////////////////////////////////////////
} //namespace VulkanAPI
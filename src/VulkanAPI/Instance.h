#pragma once

#include "VulkanAPI/VulkanAPI_fwd.inl"

namespace VulkanAPI
{
    struct QueueFamilyIndices;
    class PhysicalDevice;
    class RequiredInstanceExtensionsInfo;
}

namespace VulkanAPI
{
///////////////////////////////////////////////////////////////////////////////
class Instance {
///////////////////////////////////////////////////////////////////////////////
public:
    Instance(const std::vector<const char*>& i_validationLayers, RequiredInstanceExtensionsInfo& i_requiredInstanceExtensionsInfo);
    ~Instance();

    void SetupDebugMessenger();
    void PickPhysicalDevice();
    void CreateLogicalDevice();

private:
    void CreateDebugUtilsMessenger();
    void DestroyDebugUtilsMessenger();
    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& o_createInfo);

    bool CheckValidationLayerSupport(const std::vector<const char*>& i_validationLayers);

    int RateDeviceSuitability(VkPhysicalDevice i_device);
    bool IsDeviceSuitable(VkPhysicalDevice i_device);
    QueueFamilyIndices FindQueueFamily(VkPhysicalDevice i_device);

private:
    VkInstance m_instance;
    VkDebugUtilsMessengerEXT m_debugMessenger;

    const std::vector<const char*> k_validationLayers;
    std::unique_ptr<PhysicalDevice> m_physicalDevice;
};
///////////////////////////////////////////////////////////////////////////////
} //namespace Instance
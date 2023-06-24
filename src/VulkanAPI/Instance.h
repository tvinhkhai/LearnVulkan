#pragma once

#include "VulkanAPI/VulkanAPI_fwd.inl"

class Window;

namespace VulkanAPI
{
    struct QueueFamilyIndices;
    class PhysicalDevice;
    class RequiredInstanceExtensionsInfo;
    class WindowSurface;
}

namespace VulkanAPI
{
///////////////////////////////////////////////////////////////////////////////
class Instance {
///////////////////////////////////////////////////////////////////////////////
public:
    Instance(const std::vector<const char*>& i_validationLayers, RequiredInstanceExtensionsInfo& i_requiredInstanceExtensionsInfo);
    ~Instance();

    void CreateSurface(std::unique_ptr<Window>& i_window);
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

    std::unique_ptr<WindowSurface> m_surface;

    const std::vector<const char*> k_validationLayers;
    std::unique_ptr<PhysicalDevice> m_physicalDevice;
};
///////////////////////////////////////////////////////////////////////////////
} //namespace Instance
#pragma once

#include "VulkanAPI_fwd.inl"

class Window;
struct QueueFamilyIndices;

class VulkanAPI {
public:
    VulkanAPI();
    ~VulkanAPI();
    void CreateInstance(std::unique_ptr<Window>& i_window, bool i_enableValidationLayers);
    void SetupDebugMessenger();
    void PickPhysicalDevice();

#if defined(DEBUG)
    void PrintAvailableExtensions();
#endif

private:
    bool CheckValidationLayerSupport();
    std::vector<const char*> GetRequiredExtensions(std::unique_ptr<Window>& i_window);

    VkResult CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* i_createInfo, const VkAllocationCallbacks* i_allocator);
    void DestroyDebugUtilsMessengerEXT(const VkAllocationCallbacks* i_allocator);
    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& o_createInfo);

    int RateDeviceSuitability(VkPhysicalDevice device);
    bool IsDeviceSuitable(VkPhysicalDevice device);

    QueueFamilyIndices FindQueueFamily(VkPhysicalDevice device);

private:
    VkInstance m_instance;
    bool m_enableValidationLayers;
    VkDebugUtilsMessengerEXT m_debugMessenger;
    VkPhysicalDevice m_physicalDevice;
};
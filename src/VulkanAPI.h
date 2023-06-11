#pragma once

#include "VulkanAPI_fwd.inl"

class Window;

class VulkanAPI {
public:
    VulkanAPI();
    ~VulkanAPI();
    void CreateInstance(std::unique_ptr<Window>& i_window, bool i_enableValidationLayers);
    void SetupDebugMessenger();

#if defined(DEBUG)
    void PrintAvailableExtensions();
#endif

private:
    bool CheckValidationLayerSupport();
    std::vector<const char*> GetRequiredExtensions(std::unique_ptr<Window>& i_window);

    VkResult CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* i_createInfo, const VkAllocationCallbacks* i_allocator);
    void DestroyDebugUtilsMessengerEXT(const VkAllocationCallbacks* i_allocator);

private:
    VkInstance m_instance;
    bool m_enableValidationLayers;
    VkDebugUtilsMessengerEXT m_debugMessenger;
};
#pragma once

class Window;

namespace VulkanAPI
{
    struct QueueFamilyIndices;
    class PhysicalDevice;
    class RequiredInstanceExtensionsInfo;
    class WindowSurface;
    struct SwapChainSupportDetails;
}

namespace VulkanAPI
{
///////////////////////////////////////////////////////////////////////////////
class Instance {
///////////////////////////////////////////////////////////////////////////////
public:
    Instance(const std::vector<const char*>& i_validationLayers, RequiredInstanceExtensionsInfo& i_requiredInstanceExtensionsInfo, std::unique_ptr<Window>& i_window);
    ~Instance();

    void CreateSurface();
    void SetupDebugMessenger();
    void PickPhysicalDevice();
    void CreateLogicalDevice();
    void CreateSwapChain();
    void CreateImageViews();
    void CreateGraphicsPipeline();

private:
    void CreateDebugUtilsMessenger();
    void DestroyDebugUtilsMessenger();
    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& o_createInfo);

    bool CheckValidationLayerSupport(const std::vector<const char*>& i_validationLayers);

    int RateDeviceSuitability(VkPhysicalDevice i_device);
    bool IsDeviceSuitable(VkPhysicalDevice i_device);
    QueueFamilyIndices FindQueueFamily(VkPhysicalDevice i_device);
    bool CheckDeviceExtensionSupport(VkPhysicalDevice i_device);

    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice i_device);
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& i_availableFormats);
    VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& i_availablePresentModes);
    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& i_capabilities);
    void RetrievingSwapChainImages();

private:
    VkInstance m_instance;
    VkDebugUtilsMessengerEXT m_debugMessenger;

    std::unique_ptr<Window>& m_window;
    VkSwapchainKHR m_swapChain;
    std::vector<VkImage> m_swapChainImages;
    std::vector<VkImageView> m_swapChainImageViews;
    VkFormat m_swapChainImageFormat;
    VkExtent2D m_swapChainExtent;

    std::unique_ptr<WindowSurface> m_surface;

    const std::vector<const char*> k_validationLayers;
    std::unique_ptr<PhysicalDevice> m_physicalDevice;
};
///////////////////////////////////////////////////////////////////////////////
} //namespace Instance
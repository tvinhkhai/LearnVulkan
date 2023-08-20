#pragma once

class FileSystem;
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
    Instance(const std::vector<const char*>& i_validationLayers, RequiredInstanceExtensionsInfo& i_requiredInstanceExtensionsInfo, std::unique_ptr<Window>& i_window, std::unique_ptr<FileSystem>& i_fileSystem);
    ~Instance();

    void CreateSurface();
    void SetupDebugMessenger();
    void PickPhysicalDevice();
    void CreateLogicalDevice();
    void CreateSwapChain();
    void CreateImageViews();
    void CreateRenderPass();
    void CreateGraphicsPipeline();
    void CreateFrameBuffers();
    void CreateCommandPool();
    void CreateCommandBuffer();
    void CreateSyncObjects();

    void OnExit();

    void DrawFrame();

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

    VkShaderModule CreateShaderModule(const std::vector<char>& i_code);

    void RecordCommandBuffer(VkCommandBuffer i_commandBuffer, uint32_t i_imageIndex);

private:
    VkInstance m_instance;
    std::unique_ptr<FileSystem>& m_fileSystem;
    VkDebugUtilsMessengerEXT m_debugMessenger;

    std::unique_ptr<Window>& m_window;
    VkSwapchainKHR m_swapChain;
    std::vector<VkImage> m_swapChainImages;
    std::vector<VkImageView> m_swapChainImageViews;
    VkFormat m_swapChainImageFormat;
    VkExtent2D m_swapChainExtent;

    VkRenderPass m_renderPass;
    VkPipelineLayout m_pipelineLayout;
    VkPipeline m_graphicsPipeline;

    std::vector<VkFramebuffer> m_swapChainFramebuffers;

    VkCommandPool m_commandPool;
    VkCommandBuffer m_commandBuffer;

    VkSemaphore m_imageAvailableSemaphore;
    VkSemaphore m_renderFinishedSemaphore;
    VkFence m_inFlightFence;

    std::unique_ptr<WindowSurface> m_surface;

    const std::vector<const char*> k_validationLayers;
    std::unique_ptr<PhysicalDevice> m_physicalDevice;
};
///////////////////////////////////////////////////////////////////////////////
} //namespace Instance
#pragma once

class FileSystem;
class Window;

namespace VulkanAPI
{
class Instance;
struct RequiredInstanceExtensionsInfo;
}

namespace VulkanAPI
{
///////////////////////////////////////////////////////////////////////////////
class VulkanAPI {
///////////////////////////////////////////////////////////////////////////////
public:
    VulkanAPI();
    ~VulkanAPI();

    void CreateInstance(const std::vector<const char*>& i_validationLayers, RequiredInstanceExtensionsInfo& i_requiredInstanceExtensionsInfo, std::unique_ptr<Window>& i_window, std::unique_ptr<FileSystem>& i_fileSystem);
    void SetupDebugMessenger();
    void CreateSurface();
    void PickPhysicalDevice();
    void CreateLogicalDevice();
    void CreateSwapChain();
    void CreateImageViews();
    void CreateRenderPass();
    void CreateGraphicsPipeline();
    void CreateFrameBuffers();
    void CreateCommandPool();
    void CreateCommandBuffer();

    void PrintAvailableExtensions();

private:
    std::unique_ptr<Instance> m_instance;
};
///////////////////////////////////////////////////////////////////////////////
} //namespace VulkanAPI
#pragma once

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

    void CreateInstance(const std::vector<const char*>& i_validationLayers, RequiredInstanceExtensionsInfo& i_requiredInstanceExtensionsInfo, std::unique_ptr<Window>& i_window);
    void SetupDebugMessenger();
    void CreateSurface();
    void PickPhysicalDevice();
    void CreateLogicalDevice();
    void CreateSwapChain();

    void PrintAvailableExtensions();

private:
    std::unique_ptr<Instance> m_instance;
};
///////////////////////////////////////////////////////////////////////////////
} //namespace VulkanAPI
#pragma once

class Window;

namespace VulkanAPI
{
class Instance;
class RequiredInstanceExtensionsInfo;
}

namespace VulkanAPI
{
///////////////////////////////////////////////////////////////////////////////
class VulkanAPI {
///////////////////////////////////////////////////////////////////////////////
public:
    VulkanAPI();
    ~VulkanAPI();

    void CreateInstance(const std::vector<const char*>& i_validationLayers, RequiredInstanceExtensionsInfo& i_requiredInstanceExtensionsInfo);
    void SetupDebugMessenger();
    void CreateSurface(std::unique_ptr<Window>& i_window);
    void PickPhysicalDevice();
    void CreateLogicalDevice();

    void PrintAvailableExtensions();

private:
    std::unique_ptr<Instance> m_instance;
};
///////////////////////////////////////////////////////////////////////////////
} //namespace VulkanAPI
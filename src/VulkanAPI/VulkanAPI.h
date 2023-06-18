#pragma once

#include "VulkanAPI/VulkanAPI_fwd.inl"

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
    VulkanAPI(const std::vector<const char*>& i_validationLayers, RequiredInstanceExtensionsInfo& i_requiredInstanceExtensionsInfo);
    ~VulkanAPI();

    void PrintAvailableExtensions();

private:
    std::unique_ptr<Instance> m_instance;
};
///////////////////////////////////////////////////////////////////////////////
} //namespace VulkanAPI
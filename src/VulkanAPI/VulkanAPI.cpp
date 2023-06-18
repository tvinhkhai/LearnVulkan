#include "stdafx.h"
#include "VulkanAPI.h"

#include "VulkanAPI/Instance.h"
#include "VulkanAPI/RequiredInstanceExtensionsInfo.h"

#include <vulkan/vulkan.h>
#include <cstring>

///////////////////////////////////////////////////////////////////////////////

namespace VulkanAPI
{
///////////////////////////////////////////////////////////////////////////////

VulkanAPI::VulkanAPI(const std::vector<const char*>& i_validationLayers, RequiredInstanceExtensionsInfo& i_requiredInstanceExtensionsInfo)
    : m_instance(std::make_unique<Instance>(i_validationLayers, i_requiredInstanceExtensionsInfo))
{
}

///////////////////////////////////////////////////////////////////////////////

VulkanAPI::~VulkanAPI()
{
    m_instance.reset();
}

///////////////////////////////////////////////////////////////////////////////

void VulkanAPI::PrintAvailableExtensions()
{
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
    std::cout << "available extensions:\n";

    for (const auto& extension : extensions) {
        std::cout << '\t' << extension.extensionName << '\n';
    }
}

///////////////////////////////////////////////////////////////////////////////
} //namespace VulkanAPI
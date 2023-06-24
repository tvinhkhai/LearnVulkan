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

VulkanAPI::VulkanAPI()
    : m_instance(nullptr)
{
}

///////////////////////////////////////////////////////////////////////////////

VulkanAPI::~VulkanAPI()
{
    m_instance.reset();
}

///////////////////////////////////////////////////////////////////////////////

void VulkanAPI::CreateInstance(const std::vector<const char*>& i_validationLayers, RequiredInstanceExtensionsInfo& i_requiredInstanceExtensionsInfo)
{
    m_instance = std::make_unique<Instance>(i_validationLayers, i_requiredInstanceExtensionsInfo);
}

///////////////////////////////////////////////////////////////////////////////

void VulkanAPI::SetupDebugMessenger()
{
    m_instance->SetupDebugMessenger();
}

///////////////////////////////////////////////////////////////////////////////

void VulkanAPI::CreateSurface(std::unique_ptr<Window>& i_window)
{
    m_instance->CreateSurface(i_window);
}

///////////////////////////////////////////////////////////////////////////////

void VulkanAPI::PickPhysicalDevice()
{
    m_instance->PickPhysicalDevice();
}

///////////////////////////////////////////////////////////////////////////////

void VulkanAPI::CreateLogicalDevice()
{
    m_instance->CreateLogicalDevice();
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
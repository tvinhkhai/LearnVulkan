#include "stdafx.h"
#include "VulkanAPI.h"

#include "Window.h"

#include <vulkan/vulkan.h>

VulkanAPI::VulkanAPI()
{

}

VulkanAPI::~VulkanAPI()
{
    vkDestroyInstance(m_instance, nullptr);
}

void VulkanAPI::CreateInstance(std::unique_ptr<Window>& i_window)
{
    ///
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "LearnVulkan";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    ///
    VkInstanceCreateInfo createInfo{};
    RequiredInstanceExtensionsInfo extInfo = i_window->GetRequiredInstanceExtensionsInfo();
    
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = extInfo.Count;
    createInfo.ppEnabledExtensionNames = extInfo.Extensions;
    createInfo.enabledLayerCount = 0;

    if (vkCreateInstance(&createInfo, nullptr, &m_instance))
    {
        throw std::runtime_error("failed to create instance!");
    }
#if defined(DEBUG)
    PrintAvailableExtensions();
#endif
}

#if defined(DEBUG)
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
#endif

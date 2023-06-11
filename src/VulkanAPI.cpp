#include "stdafx.h"
#include "VulkanAPI.h"

#include "Window.h"

#include <vulkan/vulkan.h>
#include <cstring>

const std::vector<const char*> k_validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

VulkanAPI::VulkanAPI()
{

}

VulkanAPI::~VulkanAPI()
{
    vkDestroyInstance(m_instance, nullptr);
}

void VulkanAPI::CreateInstance(std::unique_ptr<Window>& i_window, bool i_enableValidationLayers)
{
    if (i_enableValidationLayers && !CheckValidationLayerSupport())
    {
        throw std::runtime_error("validation layers requested, but not available!");
    }

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
    
    if (i_enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(k_validationLayers.size());
        createInfo.ppEnabledLayerNames = k_validationLayers.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    VkResult createResult = vkCreateInstance(&createInfo, nullptr, &m_instance);
    if (createResult != VK_SUCCESS)
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

bool VulkanAPI::CheckValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : k_validationLayers)
    {
        bool layerFound = false;

        for (const VkLayerProperties& layerProperties : availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
        {
            return false;
        }
    }

    return true;
}

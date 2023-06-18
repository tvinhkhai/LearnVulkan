#include "stdafx.h"
#include "Instance.h"

#include "VulkanAPI/QueueFamilyIndices.h"
#include "Window.h"

#include <vulkan/vulkan.h>

///////////////////////////////////////////////////////////////////////////////
namespace
{
    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCb(
        VkDebugUtilsMessageSeverityFlagBitsEXT i_severity,
        VkDebugUtilsMessageTypeFlagsEXT i_type,
        const VkDebugUtilsMessengerCallbackDataEXT* i_cbData,
        void* i_userData)
    {
        std::cerr << "validation layer: " << i_cbData->pMessage << std::endl;

        return VK_FALSE;
    }
}
///////////////////////////////////////////////////////////////////////////////

namespace VulkanAPI
{
///////////////////////////////////////////////////////////////////////////////
const std::vector<const char*> k_validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};
///////////////////////////////////////////////////////////////////////////////

Instance::Instance(std::unique_ptr<Window>& i_window, bool i_enableValidationLayers)
    : m_instance(nullptr)
    , m_enableValidationLayers(false)
{
    m_enableValidationLayers = i_enableValidationLayers;

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
    std::vector<const char*> requiredExtension = GetRequiredExtensions(i_window);
    
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtension.size());
    createInfo.ppEnabledExtensionNames = requiredExtension.data();
    
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (i_enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(k_validationLayers.size());
        createInfo.ppEnabledLayerNames = k_validationLayers.data();

        PopulateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    else
    {
        createInfo.enabledLayerCount = 0;

        createInfo.pNext = nullptr;
    }

    VkResult createResult = vkCreateInstance(&createInfo, nullptr, &m_instance);
    if (createResult != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create instance!");
    }
}

///////////////////////////////////////////////////////////////////////////////

Instance::~Instance()
{
    vkDestroyInstance(m_instance, nullptr);
}

///////////////////////////////////////////////////////////////////////////////

void Instance::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& o_createInfo)
{
    o_createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    o_createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    o_createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    o_createInfo.pfnUserCallback = DebugCb;
    o_createInfo.pUserData = nullptr; //optional
}

///////////////////////////////////////////////////////////////////////////////

std::vector<const char*> Instance::GetRequiredExtensions(std::unique_ptr<Window>& i_window)
{
    RequiredInstanceExtensionsInfo extInfo = i_window->GetRequiredInstanceExtensionsInfo();

    std::vector<const char*> extensions(extInfo.Extensions, extInfo.Extensions + extInfo.Count);

    if (m_enableValidationLayers)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

///////////////////////////////////////////////////////////////////////////////
} //namespace VulkanAPI
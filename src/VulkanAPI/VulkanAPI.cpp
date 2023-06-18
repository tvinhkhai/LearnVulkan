#include "stdafx.h"
#include "VulkanAPI.h"

#include "VulkanAPI/Instance.h"
#include "VulkanAPI/QueueFamilyIndices.h"
#include "Window.h"

#include <vulkan/vulkan.h>
#include <cstring>

///////////////////////////////////////////////////////////////////////////////

namespace VulkanAPI
{
///////////////////////////////////////////////////////////////////////////////

VulkanAPI::VulkanAPI():
    m_instance(nullptr)
    , m_enableValidationLayers(false)
    , m_physicalDevice(VK_NULL_HANDLE)
    , m_logicalDevice(nullptr)
    , m_graphicsQueue(nullptr)
    , k_validationLayers{"VK_LAYER_KHRONOS_validation"}
{

}

///////////////////////////////////////////////////////////////////////////////

VulkanAPI::~VulkanAPI()
{
    vkDestroyDevice(m_logicalDevice, nullptr);
    m_instance.reset();
}

///////////////////////////////////////////////////////////////////////////////

void VulkanAPI::CreateInstance(std::unique_ptr<Window>& i_window, bool i_enableValidationLayers)
{
    if (i_enableValidationLayers && !CheckValidationLayerSupport())
    {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    m_instance = std::make_unique<Instance>(i_window, i_enableValidationLayers, k_validationLayers);
#if defined(DEBUG)
    PrintAvailableExtensions();
#endif
}

///////////////////////////////////////////////////////////////////////////////

void VulkanAPI::SetupDebugMessenger()
{
    if (!m_enableValidationLayers)
    {
        return;
    }

    VkResult result = m_instance->CreateDebugUtilsMessenger();
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}

///////////////////////////////////////////////////////////////////////////////

void VulkanAPI::PickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_instance->m_instance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_instance->m_instance, &deviceCount, devices.data());

    // Use an ordered map to automatically sort candidates by increasing score
    std::multimap<int, VkPhysicalDevice> candidates;

    for (const VkPhysicalDevice& device : devices)
    {
        int score = RateDeviceSuitability(device);
        candidates.insert(std::make_pair(score, device));
    }

    if (candidates.rbegin()->first > 0)
    {
        m_physicalDevice = candidates.rbegin()->second;
    }
    else
    {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

///////////////////////////////////////////////////////////////////////////////

void VulkanAPI::CreateLogicalDevice()
{
    assert(m_physicalDevice != VK_NULL_HANDLE);

    QueueFamilyIndices indices = FindQueueFamily(m_physicalDevice);
    assert(indices.optGraphicsFamily.has_value());

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.optGraphicsFamily.value();
    queueCreateInfo.queueCount = 1;

    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = 0;

    if (m_enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(k_validationLayers.size());
        createInfo.ppEnabledLayerNames = k_validationLayers.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    VkResult result = vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_logicalDevice);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(m_logicalDevice, indices.optGraphicsFamily.value(), 0, &m_graphicsQueue);
}

///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////

std::vector<const char*> VulkanAPI::GetRequiredExtensions(std::unique_ptr<Window>& i_window)
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

int VulkanAPI::RateDeviceSuitability(VkPhysicalDevice device)
{
    if (!IsDeviceSuitable(device))
    {
        return 0;
    }

    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    int score = 0;

    vkGetPhysicalDeviceProperties(device, &properties);
    vkGetPhysicalDeviceFeatures(device, &features);

    // Discrete GPUs have a significant performance advantage
    if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    {
        score += 1000;
    }

    // Maximum possible size of textures affects graphics quality
    score += properties.limits.maxImageDimension2D;

    // Application can't function without geometry shaders
    if (!features.geometryShader) {
        return 0;
    }

    return score;
}

///////////////////////////////////////////////////////////////////////////////

bool VulkanAPI::IsDeviceSuitable(VkPhysicalDevice device)
{
    QueueFamilyIndices indices = FindQueueFamily(device);

    return indices.IsComplete();
}

///////////////////////////////////////////////////////////////////////////////

QueueFamilyIndices VulkanAPI::FindQueueFamily(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;
    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);

    std::vector<VkQueueFamilyProperties> families(count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, families.data());

    int i = 0;
    for (const VkQueueFamilyProperties& family : families)
    {
        if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.optGraphicsFamily = i;
        }

        if (indices.IsComplete())
        {
            break;
        }

        i++;
    }

    return indices;
}

///////////////////////////////////////////////////////////////////////////////
} //namespace VulkanAPI
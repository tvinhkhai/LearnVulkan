#include "stdafx.h"
#include "VulkanAPI.h"

#include "QueueFamilyIndices.h"
#include "Window.h"

#include <vulkan/vulkan.h>
#include <cstring>

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCb(
    VkDebugUtilsMessageSeverityFlagBitsEXT i_severity,
    VkDebugUtilsMessageTypeFlagsEXT i_type,
    const VkDebugUtilsMessengerCallbackDataEXT* i_cbData,
    void* i_userData)
{
    std::cerr << "validation layer: " << i_cbData->pMessage << std::endl;

    return VK_FALSE;
}

const std::vector<const char*> k_validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

VulkanAPI::VulkanAPI():
    m_instance(nullptr)
    , m_enableValidationLayers(false)
    , m_debugMessenger(nullptr)
    , m_physicalDevice(VK_NULL_HANDLE)
    , m_logicalDevice(nullptr)
    , m_graphicsQueue(nullptr)
{

}

VulkanAPI::~VulkanAPI()
{
    vkDestroyDevice(m_logicalDevice, nullptr);
    if (m_enableValidationLayers)
    {
        DestroyDebugUtilsMessengerEXT(nullptr);
    }
    vkDestroyInstance(m_instance, nullptr);
}

void VulkanAPI::CreateInstance(std::unique_ptr<Window>& i_window, bool i_enableValidationLayers)
{
    if (i_enableValidationLayers && !CheckValidationLayerSupport())
    {
        throw std::runtime_error("validation layers requested, but not available!");
    }

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
#if defined(DEBUG)
    PrintAvailableExtensions();
#endif
}

void VulkanAPI::SetupDebugMessenger()
{
    if (!m_enableValidationLayers)
    {
        return;
    }

    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    PopulateDebugMessengerCreateInfo(createInfo);

    VkResult result = CreateDebugUtilsMessengerEXT(&createInfo, nullptr);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}

void VulkanAPI::PickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

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

VkResult VulkanAPI::CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* i_createInfo, const VkAllocationCallbacks* i_allocator)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        return func(m_instance, i_createInfo, nullptr, &m_debugMessenger);
    }
    else
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void VulkanAPI::DestroyDebugUtilsMessengerEXT(const VkAllocationCallbacks* i_allocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(m_instance, m_debugMessenger, i_allocator);
    }
}

void VulkanAPI::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& o_createInfo)
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

bool VulkanAPI::IsDeviceSuitable(VkPhysicalDevice device)
{
    QueueFamilyIndices indices = FindQueueFamily(device);

    return indices.IsComplete();
}

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

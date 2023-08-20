#include "stdafx.h"
#include "Instance.h"

#include "VulkanAPI/LogicalDevice.h"
#include "VulkanAPI/PhysicalDevice.h"
#include "VulkanAPI/QueueFamilyIndices.h"
#include "VulkanAPI/RequiredInstanceExtensionsInfo.h"
#include "VulkanAPI/SwapChainSupportDetails.h"
#include "VulkanAPI/WindowSurface.h"

#include "Window.h"

#include <cstdint>
#include <limits>
#include <algorithm>

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
const std::vector<const char*> k_deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
///////////////////////////////////////////////////////////////////////////////

Instance::Instance(const std::vector<const char*>& i_validationLayers, RequiredInstanceExtensionsInfo& i_requiredInstanceExtensionsInfo, std::unique_ptr<Window>& i_window)
    : m_instance(nullptr)
    , m_window(i_window)
    , m_surface(nullptr)
    , m_debugMessenger(nullptr)
    , k_validationLayers(i_validationLayers)
    , m_physicalDevice(nullptr)
{
    if (!i_validationLayers.empty() && !CheckValidationLayerSupport(i_validationLayers))
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
    std::vector<const char*> requiredExtension(i_requiredInstanceExtensionsInfo.Extensions, i_requiredInstanceExtensionsInfo.Extensions + i_requiredInstanceExtensionsInfo.Count);
    if (!k_validationLayers.empty())
    {
        requiredExtension.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtension.size());
    createInfo.ppEnabledExtensionNames = requiredExtension.data();
    
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (k_validationLayers.empty())
    {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }
    else
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(k_validationLayers.size());
        createInfo.ppEnabledLayerNames = k_validationLayers.data();

        PopulateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
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
    LogicalDevice* logicalDevice = m_physicalDevice->GetLogicalDevice();
    assert(logicalDevice != nullptr);
    vkDestroySwapchainKHR(logicalDevice->GetDevice(), m_swapChain, nullptr);
    m_physicalDevice.reset();
    DestroyDebugUtilsMessenger();
    m_surface.reset();
    vkDestroyInstance(m_instance, nullptr);
}

///////////////////////////////////////////////////////////////////////////////

void Instance::CreateSurface()
{
    VkSurfaceKHR surface = m_window->CreateVulkanSurface(m_instance);
    m_surface = std::make_unique<WindowSurface>(m_instance, surface);
}

///////////////////////////////////////////////////////////////////////////////

void Instance::SetupDebugMessenger()
{
    if (!k_validationLayers.empty())
    {
        CreateDebugUtilsMessenger();
    }
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

void Instance::CreateDebugUtilsMessenger()
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    PopulateDebugMessengerCreateInfo(createInfo);

    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        VkResult result = func(m_instance, &createInfo, nullptr, &m_debugMessenger);
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to set up debug messenger!");
        }
    }
    else
    {
        throw std::runtime_error("failed to set up debug messenger! Extension not present!");
    }
}

///////////////////////////////////////////////////////////////////////////////

void Instance::DestroyDebugUtilsMessenger()
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr && m_debugMessenger != nullptr) {
        func(m_instance, m_debugMessenger, nullptr);
    }
}

///////////////////////////////////////////////////////////////////////////////

bool Instance::CheckValidationLayerSupport(const std::vector<const char*>& i_validationLayers)
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : i_validationLayers)
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

void Instance::PickPhysicalDevice()
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
        VkPhysicalDevice device = candidates.rbegin()->second;
        QueueFamilyIndices indicies = FindQueueFamily(device);
        m_physicalDevice = std::make_unique<PhysicalDevice>(device, indicies);
    }
    else
    {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

///////////////////////////////////////////////////////////////////////////////

void Instance::CreateLogicalDevice()
{
    m_physicalDevice->CreateLogicalDevice(k_validationLayers, k_deviceExtensions);
}

///////////////////////////////////////////////////////////////////////////////

void Instance::CreateSwapChain()
{
    SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(m_physicalDevice->GetDevice());

    VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = m_surface->GetSurface();

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = m_physicalDevice->GetQueueFamilyIndices();
    uint32_t queueFamilyIndices[] = { indices.optGraphicsFamily.value(), indices.optPresentFamily.value() };

    if (queueFamilyIndices[0] != queueFamilyIndices[1])
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;
    
    LogicalDevice* logicalDevice = m_physicalDevice->GetLogicalDevice();
    assert(logicalDevice != nullptr);
    if (vkCreateSwapchainKHR(logicalDevice->GetDevice(), &createInfo, nullptr, &m_swapChain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }
}

///////////////////////////////////////////////////////////////////////////////

int Instance::RateDeviceSuitability(VkPhysicalDevice i_device)
{
    if (!IsDeviceSuitable(i_device))
    {
        return 0;
    }

    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    int score = 0;

    vkGetPhysicalDeviceProperties(i_device, &properties);
    vkGetPhysicalDeviceFeatures(i_device, &features);

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

bool Instance::IsDeviceSuitable(VkPhysicalDevice i_device)
{
    QueueFamilyIndices indices = FindQueueFamily(i_device);

    bool extensionsSupported = CheckDeviceExtensionSupport(i_device);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(i_device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return indices.IsComplete() && extensionsSupported && swapChainAdequate;
}

///////////////////////////////////////////////////////////////////////////////

QueueFamilyIndices Instance::FindQueueFamily(VkPhysicalDevice i_device)
{
    QueueFamilyIndices indices;
    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(i_device, &count, nullptr);

    std::vector<VkQueueFamilyProperties> families(count);
    vkGetPhysicalDeviceQueueFamilyProperties(i_device, &count, families.data());

    int i = 0;
    for (const VkQueueFamilyProperties& family : families)
    {
        if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.optGraphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(i_device, i, m_surface->GetSurface(), &presentSupport);
        if (presentSupport)
        {
            indices.optPresentFamily = i;
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

bool Instance::CheckDeviceExtensionSupport(VkPhysicalDevice i_device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(i_device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(i_device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(k_deviceExtensions.begin(), k_deviceExtensions.end());

    for (const auto& extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

///////////////////////////////////////////////////////////////////////////////

SwapChainSupportDetails Instance::QuerySwapChainSupport(VkPhysicalDevice i_device)
{
    SwapChainSupportDetails details;
    VkSurfaceKHR surface = m_surface->GetSurface();

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(i_device, surface, &details.capabilities);

    uint32_t formatCount;
    {
        vkGetPhysicalDeviceSurfaceFormatsKHR(i_device, surface, &formatCount, nullptr);

        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(i_device, surface, &formatCount, details.formats.data());
        }
    }

    uint32_t presentModeCount;
    {
        vkGetPhysicalDeviceSurfacePresentModesKHR(i_device, surface, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(i_device, surface, &presentModeCount, details.presentModes.data());
        }
    }

    return details;
}

///////////////////////////////////////////////////////////////////////////////

VkSurfaceFormatKHR Instance::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& i_availableFormats)
{
    for (const auto& availableFormat : i_availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return i_availableFormats[0];
}

///////////////////////////////////////////////////////////////////////////////

VkPresentModeKHR Instance::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& i_availablePresentModes)
{
    for (const auto& availablePresentMode : i_availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

///////////////////////////////////////////////////////////////////////////////

VkExtent2D Instance::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& i_capabilities)
{
    if (i_capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) 
    {
        return i_capabilities.currentExtent;
    }
    else 
    {
        Window::Size framebufferSize = m_window->GetFramebufferSize();
        VkExtent2D actualExtent = {
            static_cast<uint32_t>(framebufferSize.width),
            static_cast<uint32_t>(framebufferSize.height)
        };

        actualExtent.width = std::clamp(actualExtent.width, i_capabilities.minImageExtent.width, i_capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, i_capabilities.minImageExtent.height, i_capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

///////////////////////////////////////////////////////////////////////////////
} //namespace Instance
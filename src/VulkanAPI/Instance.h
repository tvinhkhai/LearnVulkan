#pragma once

#include "VulkanAPI/VulkanAPI_fwd.inl"

class Window;

namespace VulkanAPI
{
///////////////////////////////////////////////////////////////////////////////
class Instance {
///////////////////////////////////////////////////////////////////////////////
public:
    Instance(std::unique_ptr<Window>& i_window, bool i_enableValidationLayers, const std::vector<const char*>& i_validationLayers);
    ~Instance();

    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& o_createInfo);
    std::vector<const char*> GetRequiredExtensions(std::unique_ptr<Window>& i_window);

    VkResult CreateDebugUtilsMessenger();

    VkInstance m_instance; //todo: move to private
private:
    void DestroyDebugUtilsMessenger();

private:
    bool m_enableValidationLayers;
    VkDebugUtilsMessengerEXT m_debugMessenger;

    const std::vector<const char*>& k_validationLayers;
};
///////////////////////////////////////////////////////////////////////////////
} //namespace Instance
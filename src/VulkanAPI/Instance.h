#pragma once

#include "VulkanAPI/VulkanAPI_fwd.inl"

class Window;

namespace VulkanAPI
{
///////////////////////////////////////////////////////////////////////////////
class Instance {
///////////////////////////////////////////////////////////////////////////////
public:
    Instance(std::unique_ptr<Window>& i_window, bool i_enableValidationLayers);
    ~Instance();

    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& o_createInfo);
    std::vector<const char*> GetRequiredExtensions(std::unique_ptr<Window>& i_window);

    VkInstance m_instance; //todo make this private after done refactor
private:
    bool m_enableValidationLayers;
};
///////////////////////////////////////////////////////////////////////////////
} //namespace Instance
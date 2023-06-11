#pragma once

class Window;

//forward declare vulkan instance, is this the proper way?
struct VkInstance_T;
typedef struct VkInstance_T* VkInstance;

class VulkanAPI {
public:
    VulkanAPI();
    ~VulkanAPI();
    void CreateInstance(std::unique_ptr<Window>& i_window, bool i_enableValidationLayers);

#if defined(DEBUG)
    void PrintAvailableExtensions();
#endif

private:
    bool CheckValidationLayerSupport();

private:
    VkInstance m_instance;
};
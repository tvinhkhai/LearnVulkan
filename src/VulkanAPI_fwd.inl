#pragma once

//forward declare vulkan instance, is this the proper way?
#define VK_FW_DECLARATION(object) \
struct object##_T;\
typedef struct object##_T* object;

VK_FW_DECLARATION(VkInstance);
VK_FW_DECLARATION(VkDebugUtilsMessengerEXT);
VK_FW_DECLARATION(VkPhysicalDevice);
VK_FW_DECLARATION(VkDevice);
VK_FW_DECLARATION(VkQueue);
struct VkDebugUtilsMessengerCreateInfoEXT;
struct VkAllocationCallbacks;
enum VkResult;
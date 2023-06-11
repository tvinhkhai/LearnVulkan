#pragma once

//forward declare vulkan instance, is this the proper way?
struct VkInstance_T;
typedef struct VkInstance_T* VkInstance;
struct VkDebugUtilsMessengerEXT_T;
typedef struct VkDebugUtilsMessengerEXT_T* VkDebugUtilsMessengerEXT;
struct VkDebugUtilsMessengerCreateInfoEXT;
struct VkAllocationCallbacks;
enum VkResult;
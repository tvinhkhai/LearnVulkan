#include "stdafx.h"
#include "PhysicalDevice.h"

#include "VulkanAPI/LogicalDevice.h"
//#include "VulkanAPI/QueueFamilyIndices.h"

#include <vulkan/vulkan.h>

namespace VulkanAPI
{
///////////////////////////////////////////////////////////////////////////////

PhysicalDevice::PhysicalDevice(VkPhysicalDevice i_device, QueueFamilyIndices& i_queueFamilyIndices)
	: m_device(i_device)
	, m_logicalDevice(nullptr)
	, m_queueFamilyIndices(i_queueFamilyIndices)
{

}

///////////////////////////////////////////////////////////////////////////////

PhysicalDevice::~PhysicalDevice()
{

}

///////////////////////////////////////////////////////////////////////////////

void PhysicalDevice::CreateLogicalDevice(const std::vector<const char*>& i_validationLayers)
{
	assert(m_queueFamilyIndices.IsComplete());

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { m_queueFamilyIndices.optGraphicsFamily.value(), m_queueFamilyIndices.optPresentFamily.value() };
	float queuePriority = 1.0f;

	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		queueCreateInfos.push_back(queueCreateInfo);
	}

	

	VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = 0;

	if (i_validationLayers.empty())
	{
	    createInfo.enabledLayerCount = 0;
	}
	else
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(i_validationLayers.size());
		createInfo.ppEnabledLayerNames = i_validationLayers.data();
	}
	VkDevice logicalDevice;
	VkResult result = vkCreateDevice(m_device, &createInfo, nullptr, &logicalDevice);
	if (result != VK_SUCCESS) {
	    throw std::runtime_error("failed to create logical device!");
	}

	m_logicalDevice = std::make_unique<LogicalDevice>(logicalDevice, m_queueFamilyIndices);
}

///////////////////////////////////////////////////////////////////////////////
} //namespace PhysicalDevice
#pragma once

namespace VulkanAPI
{
///////////////////////////////////////////////////////////////////////////////
struct QueueFamilyIndices 
{
	std::optional<uint32_t> optGraphicsFamily;
	std::optional<uint32_t> optPresentFamily;

	inline bool IsComplete()
	{
		return optGraphicsFamily.has_value() && optPresentFamily.has_value();
	}
};
///////////////////////////////////////////////////////////////////////////////
} //namespace VulkanAPI
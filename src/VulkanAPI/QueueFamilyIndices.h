#pragma once

struct QueueFamilyIndices 
{
	std::optional<uint32_t> optGraphicsFamily;

	inline bool IsComplete()
	{
		return optGraphicsFamily.has_value();
	}
};
#pragma once

namespace VulkanAPI
{
///////////////////////////////////////////////////////////////////////////////
struct RequiredInstanceExtensionsInfo
{
    uint32_t Count = 0;
    const char** Extensions;
};
///////////////////////////////////////////////////////////////////////////////
} //namespace VulkanAPI
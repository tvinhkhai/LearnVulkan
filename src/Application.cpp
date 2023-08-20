#include "stdafx.h"
#include "Application.h"

#include "FileSystem.h"
#include "Window.h"
#include "VulkanAPI/VulkanAPI.h"
#include "VulkanAPI/RequiredInstanceExtensionsInfo.h"

Application::Application()
    :m_window(std::make_unique<Window>())
    , m_vulkanAPI(nullptr)
    , m_fileSystem(std::make_unique<FileSystem>())
{
}

///////////////////////////////////////////////////////////////////////////////

Application::~Application()
{
}

///////////////////////////////////////////////////////////////////////////////

void Application::Run()
{
    InitVulkan();
    MainLoop();
    Cleanup();
}

///////////////////////////////////////////////////////////////////////////////

void Application::InitVulkan()
{
#if defined(NDEBUG)
    const std::vector<const char*> k_validationLayers;
#else
    const std::vector<const char*> k_validationLayers {"VK_LAYER_KHRONOS_validation"};
#endif
    VulkanAPI::RequiredInstanceExtensionsInfo info = m_window->GetRequiredInstanceExtensionsInfo();
    m_vulkanAPI = std::make_unique<VulkanAPI::VulkanAPI>();
    m_vulkanAPI->CreateInstance(k_validationLayers, info, m_window, m_fileSystem);
    m_vulkanAPI->SetupDebugMessenger();
    m_vulkanAPI->CreateSurface();
    m_vulkanAPI->PickPhysicalDevice();
    m_vulkanAPI->CreateLogicalDevice();
    m_vulkanAPI->CreateSwapChain();
    m_vulkanAPI->CreateImageViews();
    m_vulkanAPI->CreateGraphicsPipeline();
}

///////////////////////////////////////////////////////////////////////////////

void Application::MainLoop()
{
    while (!m_window->IsExiting())
    {
        m_window->Update();
    }
}

///////////////////////////////////////////////////////////////////////////////

void Application::Cleanup()
{

}

///////////////////////////////////////////////////////////////////////////////
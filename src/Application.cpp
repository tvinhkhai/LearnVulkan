#include "stdafx.h"
#include "Application.h"

#include "Window.h"
#include "VulkanAPI.h"


#if defined(NDEBUG)
constexpr bool k_wantsValidationLayers = false;
#else
constexpr bool k_wantsValidationLayers = true;
#endif //NDEBUG

Application::Application()
    :m_window(std::make_unique<Window>())
    , m_vulkanAPI(std::make_unique<VulkanAPI>())
{
}

Application::~Application()
{
}

void Application::Run()
{
    InitVulkan();
    MainLoop();
    Cleanup();
}

void Application::InitVulkan()
{
    m_vulkanAPI->CreateInstance(m_window, k_wantsValidationLayers);
    m_vulkanAPI->SetupDebugMessenger();
}

void Application::MainLoop()
{
    while (!m_window->IsExiting())
    {
        m_window->Update();
    }
}

void Application::Cleanup()
{

}
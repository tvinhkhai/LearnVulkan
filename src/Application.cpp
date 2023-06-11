#include "stdafx.h"
#include "Application.h"

#include "Window.h"
#include "VulkanAPI.h"

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
    m_vulkanAPI->CreateInstance(m_window);
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
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

void Application::run()
{
    initVulkan();
    mainLoop();
    cleanup();
}

void Application::initVulkan()
{
    m_vulkanAPI->CreateInstance(m_window);
}

void Application::mainLoop()
{
    while (!m_window->IsExiting())
    {
        m_window->Update();
    }
}

void Application::cleanup()
{

}
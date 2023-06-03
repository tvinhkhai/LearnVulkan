#include "stdafx.h"
#include "Application.h"

#include "Window.h"

Application::Application()
    :m_window(std::make_unique<Window>())
{
}

Application::~Application()
{
    m_window.reset();
}

void Application::run()
{
    initVulkan();
    mainLoop();
    cleanup();
}

void Application::initVulkan()
{

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
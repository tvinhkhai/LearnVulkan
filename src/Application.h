#pragma once

class Window;
namespace VulkanAPI
{
class VulkanAPI;
}

class Application {
///////////////////////////////////////////////////////////////////////////////
public:
    Application();
    ~Application();
    void Run();

private:
    void InitVulkan();
    void MainLoop();
    void Cleanup();

private:
    std::unique_ptr<Window> m_window;
    std::unique_ptr<VulkanAPI::VulkanAPI> m_vulkanAPI;

///////////////////////////////////////////////////////////////////////////////
};
#pragma once

class Window;
class VulkanAPI;

class Application {
public:
    Application();
    ~Application();
    void run();

private:
    void initVulkan();
    void mainLoop();
    void cleanup();

private:
    std::unique_ptr<Window> m_window;
    std::unique_ptr<VulkanAPI> m_vulkanAPI;
};
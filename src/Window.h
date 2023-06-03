#pragma once

struct GLFWwindow;

struct RequiredInstanceExtensionsInfo
{
    uint32_t Count = 0;
    const char** Extensions;
};

class Window {
public:
    Window();
    ~Window();

    bool IsExiting();
    void Update();

    RequiredInstanceExtensionsInfo GetRequiredInstanceExtensionsInfo();

private:
    GLFWwindow* m_window;
};
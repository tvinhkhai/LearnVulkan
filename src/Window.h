#pragma once

struct GLFWwindow;

class Window {
public:
    Window();
    ~Window();

    bool IsExiting();
    void Update();

private:
    GLFWwindow* m_window;
};
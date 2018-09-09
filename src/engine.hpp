#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <utility>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "key_codes.hpp"
#include "program.hpp"
#include "exceptions.hpp"

class Engine {
private:
    using KeyCallbackFunc = void(*)();
    using MousePositionFunc = void(*)(double, double);
    using MouseButtonFunc = void(*)();
    struct GLFWWindowDeleter {
        void operator()(GLFWwindow* window) {
            glfwDestroyWindow(window);
        };
    };

    struct KeyCallbacks {
        KeyCallbackFunc m_on_press = nullptr;
        KeyCallbackFunc m_on_repeat = nullptr;
        KeyCallbackFunc m_on_release = nullptr;
    };

    struct MouseCallbacks {
        MouseButtonFunc m_on_press = nullptr;
        MouseButtonFunc m_on_repeat = nullptr;
        MouseButtonFunc m_on_release = nullptr;
    };

private:
    std::unique_ptr<GLFWwindow, GLFWWindowDeleter> m_active_window;
    Program* m_active_program = nullptr;
    std::vector<std::unique_ptr<Program>> m_programs;
    std::unordered_map<int, KeyCallbacks> m_key_callbacks;
    MousePositionFunc m_mouse_position_callback;
    std::unordered_map<int, MouseCallbacks> m_mouse_callbacks;
    int m_width, m_height;

    void on_key_callback(int key, int action);
    static void _on_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void _on_mouse_position(GLFWwindow* window, double x, double y);
    void on_mouse_position(double x, double y);
    void on_mouse_button(int key, int action);
    static void _on_mouse_button(GLFWwindow* window, int key, int action, int mods);
    void on_resize(int width, int height);
    static void _on_resize(GLFWwindow* window, int width, int height);
    void create_new_window(int width, int height, const std::string& title);
    void init();

public:
    Engine(int width, int height, const std::string& title);
    ~Engine();
    uint32_t load_new_program(const std::string& vertex_shader_filename, const std::string& fragment_shader_filename);
    void set_program(uint32_t program_id);
    void render();
    void set_on_key_press_callback(int key, KeyCallbackFunc func);
    void set_on_key_repeat_callback(int key, KeyCallbackFunc func);
    void set_on_key_release_callback(int key, KeyCallbackFunc func);
    void set_mouse_position_callback(MousePositionFunc func);
    void set_on_mouse_press_callback(int key, MouseButtonFunc);
    void set_on_mouse_repeat_callback(int key, MouseButtonFunc);
    void set_on_mouse_release_callback(int key, MouseButtonFunc);
    bool stop();
    void finish_frame();
};

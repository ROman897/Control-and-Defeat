#include "key_codes.hpp"

class Engine {
private:
    using KeyCallbackFunc = void(*)();
    using MousePositionFunc = void(*)(double, double);
    using MouseButtonCallback = void(*)()
    std::unique_ptr<GLFWwindow, glfwDestroyWindow> m_active_window;
    Program* m_active_program = nullptr;
    std::vector<std::unique_ptr<Program>> m_programs;
    std::unordered_map<std::pair<int, KeyStates>, KeyCallbackFunc> m_key_callbacks;
    MousePositionFunc m_mouse_position_callback;
    std::unordered_map<std::pair<int, KeyStates>, MouseButtonCallback> m_mouse_callbacks;
    void on_key_callback(int key, int action);
    static void _on_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void _on_mouse_position(GLFWwindow* window, double x, double y);
    static void on_mouse_position(double x, double y);
    void on_mouse_button(int key, int action);
    static void _on_mouse_button(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
    enum KeyStates {
        KEY_PRESS = GLFW_PRESS,
        KEY_REPEAT = GLFW_REPEAT,
        KEY_RELEASE = GLFW_RELEASE
    };
    void create_new_window(int width, int height, const std::string& title);
    uint32_t load_new_program(const std::string& vertex_shader_filename, const std::string& fragment_shader_filename);
    void set_program(uint32_t program_id);
    void init();
    void render();
    void set_key_callback(int key, KeyStates action, KeyCallbackFunc func);
    void set_mouse_position_callback(MousePositionFunc func);
    void set_mouse_button_callback(int key, KeyStates action, MouseButtonCallback);
};

#include <cassert>

#include "engine.hpp"
#include "exceptions.hpp"

void Engine::create_new_window(int width, int height, const std::string& title) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    /* TODO(Roman): later query for all monitors and start in full screen mode */
    GLFWwindow* new_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!new_window) {
        throw GraphicError("could not create window!");
    }
    m_active_window.reset(new_window);
    glfwSetWindowUserPointer(new_window, this);
    glfwSetKeyCallback(new_window, &_on_key_callback);
    glfwSetCursorPosCallback(new_window, &_on_mouse_position);
    glfwSetMouseButtonCallback(new_window, &_on_mouse_button);
}

void Engine::on_key_callback(int key, int action) {
    auto key_it = m_key_callbacks.find(key);
    if (key_it == m_key_callbacks.end()) {
        return;
    }
    switch (action) {
        case GLFW_PRESS:
            if (key_it->second.m_on_press != nullptr) {
                key_it->second.m_on_press();
            }
            break;
        case GLFW_REPEAT:
            if (key_it->second.m_on_repeat != nullptr) {
                key_it->second.m_on_repeat();
            }
            break;
        case GLFW_RELEASE:
            if (key_it->second.m_on_release != nullptr) {
                key_it->second.m_on_release();
            }
            break;
        default:
            assert(false);
    }
}

void Engine::_on_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Engine* this_ptr = static_cast<Engine*>(glfwGetWindowUserPointer(window));
    this_ptr->on_key_callback(key, action);
}

void Engine::on_mouse_position(double x, double y) {
    if (m_mouse_position_callback != nullptr) {
        m_mouse_position_callback(x, y);
    }
}

void Engine::_on_mouse_position(GLFWwindow* window, double x, double y) {
    Engine* this_ptr = static_cast<Engine*>(glfwGetWindowUserPointer(window));
    this_ptr->on_mouse_position(x, y);
}

void Engine::on_mouse_button(int key, int action) {
    auto mouse_it = m_mouse_callbacks.find(key);
    if (mouse_it == m_mouse_callbacks.end()) {
        return;
    }
    switch (action) {
        case GLFW_PRESS:
            if (mouse_it->second.m_on_press != nullptr) {
                mouse_it->second.m_on_press();
            }
            break;
        case GLFW_REPEAT:
            if (mouse_it->second.m_on_repeat != nullptr) {
                mouse_it->second.m_on_repeat();
            }
            break;
        case GLFW_RELEASE:
            if (mouse_it->second.m_on_release != nullptr) {
                mouse_it->second.m_on_release();
            }
            break;
        default:
            assert(false);
    }
}

void Engine::_on_mouse_button(GLFWwindow* window, int key, int action, int mods) {
    Engine* this_ptr = static_cast<Engine*>(glfwGetWindowUserPointer(window));
    this_ptr->on_mouse_button(key, action);
}

uint32_t Engine::load_new_program(const std::string& vertex_shader_filename, const std::string& fragment_shader_filename) {
    m_programs.push_back(std::make_unique<Program>(vertex_shader_filename, fragment_shader_filename));
    return m_programs.size() - 1;
}

void Engine::set_program(uint32_t program_id) {
    assert (program_id < m_programs.size());
    m_active_program = m_programs[program_id].get();
    m_active_program->use();
}

void Engine::init() {
    if (!gladLoadGL()) {
        throw GraphicError("Could not initialize opengl functions");
    }
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
}

void Engine::set_on_key_press_callback(int key, KeyCallbackFunc func) {
    m_key_callbacks[key].m_on_press = func;
}
void Engine::set_on_key_repeat_callback(int key, KeyCallbackFunc func) {
    m_key_callbacks[key].m_on_repeat= func;
}
void Engine::set_on_key_release_callback(int key, KeyCallbackFunc func) {
    m_key_callbacks[key].m_on_release = func;
}

void Engine::set_mouse_position_callback(MousePositionFunc func) {
    m_mouse_position_callback = func;
}

void Engine::set_on_mouse_press_callback(int key, MouseButtonFunc func) {
    m_mouse_callbacks[key].m_on_press = func;
}

void Engine::set_on_mouse_repeat_callback(int key, MouseButtonFunc func) {
    m_mouse_callbacks[key].m_on_repeat = func;
}

void Engine::set_on_mouse_release_callback(int key, MouseButtonFunc func) {
    m_mouse_callbacks[key].m_on_release = func;
}

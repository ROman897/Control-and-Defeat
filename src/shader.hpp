#pragma once
#include <string>

#include <glad/glad.h>
class Shader {
public:
    enum class CompilationStatus {
        SUCCESS,
        FAIL,
        FILE_NOT_FOUND
    };

protected:
    GLuing m_shader_id;
    CompilationStatus m_compilation_status;
    Shader() = default;

public:
    int get_shader_id() {
        return m_shader_id;
    }
    std::string get_log();
    CompilationStatus get_compilation_status() {
        return m_compilation_status;
    }
    virtual ~Shader() {
        glDeleteShader(m_shader_id);
    }
};

class VertexShader {
public:
    VertexShader(const std::string& filename);
};

class FragmentShader {
public:
    FragmentShader(const std::string& filename);
};

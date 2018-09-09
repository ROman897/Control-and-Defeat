#pragma once
#include <string>

#include <glad/glad.h>
class Shader {
private:
    GLuint m_shader_id;
protected:
    Shader() = default;
    void compile(const std::string& filename, GLenum shader_type);

public:
    int get_shader_id() {
        return m_shader_id;
    }
    virtual ~Shader() {
        glDeleteShader(m_shader_id);
    }
};

class VertexShader : public Shader {
public:
    VertexShader(const std::string& filename);
};

class FragmentShader : public Shader {
public:
    FragmentShader(const std::string& filename);
};

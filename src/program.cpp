#include <iostream>

#include "program.hpp"
#include "shader.hpp"
#include "exceptions.hpp"

#include <glad/glad.h>

void Program::compile(const std::string& vertex_file, const std::string& fragment_file) {
    VertexShader vertex_shader(vertex_file);
    FragmentShader fragment_shader(fragment_file);

    m_program_id = glCreateProgram();
    glAttachShader(m_program_id, vertex_shader.get_shader_id());
    glAttachShader(m_program_id, fragment_shader.get_shader_id());

    glLinkProgram(m_program_id);
    GLint success;
    char info_log[1024];
    glGetProgramiv(m_program_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_program_id, 1024, NULL, info_log);
        throw GraphicError(info_log);
    }
}



#include <iostream>

#include "program.hpp"
#include "shader.hpp"

void Program::compile(const std::string& vertex_file, const std::string& fragment_file) {
    Shader vertex_shader(vertex_file);
    if (vertex_shader.get_compilation_status() == Shader::CompilationStatus::FILE_NOT_FOUND) {
        std::cerr << "file " << vertex_file << "not found" << std::endl;
        return;
    }
    if (vertex_shader.get_compilation_status() == Shader::CompilationStatus::FAIL) {
        std::cerr << "compilation of vertex shader " << vertex_file << "failed: " 
            << vertex_shader.get_log() << std::endl;
        return;
    }
    Shader fragment_shader(fragment_file);
    if (fragment_shader.get_compilation_status() == Shader::CompilationStatus::FILE_NOT_FOUND) {
        std::cerr << "file " << fragment_file << "not found" << std::endl;
        return;
    }
    if (fragment_shader.get_compilation_status() == Shader::CompilationStatus::FAIL) {
        std::cerr << "compilation of vertex shader " << fragment_file << "failed: " 
            << fragment_shader.get_log() << std::endl;
        return;
    }

    m_program_id = glCreateProgram();
    glAtachShader(m_program_id, vertex_shader.get_shader_id());
    glAtachShader(m_program_id, fragment_shader.get_shader_id());

    glLinkProgram(m_program_id);
    GLint success;
    char infoLog[1024];
    glGetProgramiv(m_program_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_program_id, 1024, NULL, infoLog);
        std::cerr << "Linking of program failed: " << infolog << std::endl;
    }
}



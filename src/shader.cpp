#include <fstream>

#include "shader.hpp"

void Shader::compile(const std::string& filename, GLenum shader_type) {
    std::ifstream source(filename);
    if (!source.is_open()) {
        m_compilation_status = CompilationStatus::FILE_NOT_FOUND;
        return;
    }
    std::string content(std::istreambuf_iterator<char>(shader_src), std::istreambuf_iterator<char>());
    m_shader_id = glCreateShader(shader_type);
    glShaderSource(m_shader_id, 1, &source.c_str(), NULL);
    glCompileShader(m_shader_id);
    GLint success;
    GLchar infoLog[1024];
    glGetShaderiv(m_shader_id, GL_COMPILE_STATUS, &success);
    m_compilation_status = success ? CompilationStatus::SUCCESS : CompilationStatus::FAIL;
}

std::string Shader::get_log() {
    int log_len = 0;
    glGetShaderiv(m_shader_id, GL_INFO_LOG_LENGTH, &log_len);
    assert(log_len > 0);
    std::string log_message(log_len);
    glShaderInfoLog(_shader_id, log_len, nullptr, &log_message[0]);
    return log_message;
}

VertexShader::VertexShader(const std::string& filename) {
    compile(filename, GL_VERTEX_SHADER);
}

FragmentShader::FragmentShader(const std::string& filename) {
    compile(filename, GL_FRAGMENT_SHADER);
}


#include <fstream>
#include <cassert>

#include "shader.hpp"
#include "exceptions.hpp"

void Shader::compile(const std::string& filename, GLenum shader_type) {
    std::ifstream source(filename);
    if (!source.is_open()) {
        throw GraphicError("File" + filename + "not found");
    }
    std::string content = std::string(std::istreambuf_iterator<char>(source), std::istreambuf_iterator<char>());
    const char* content_ptr = content.c_str();
    m_shader_id = glCreateShader(shader_type);
    glShaderSource(m_shader_id, 1, &content_ptr, NULL);
    glCompileShader(m_shader_id);
    GLint success;
    glGetShaderiv(m_shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        int log_len = 0;
        glGetShaderiv(m_shader_id, GL_INFO_LOG_LENGTH, &log_len);
        std::string info_log;
        info_log.resize(log_len);
        glGetShaderInfoLog(m_shader_id, log_len, nullptr, &info_log[0]);
        throw GraphicError(info_log);
    }
}

VertexShader::VertexShader(const std::string& filename) {
    compile(filename, GL_VERTEX_SHADER);
}

FragmentShader::FragmentShader(const std::string& filename) {
    compile(filename, GL_FRAGMENT_SHADER);
}


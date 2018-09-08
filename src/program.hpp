#include <string>

#include <glad/glad.h>

class Program {
protected:
    GLuint m_program_id;

private:
    void compile(const std::string& vertex_file, const std::string& fragment_file);

public:
    Program& use() {
        glUseProgram(m_program_id);
        return *this;
    }

    Program(const std::string& vertex_file, const std::string& fragment_file) {
        compile(vertex_file, fragment_file);
    }

    ~Program() {
        glDeleteProgram(m_program_id);
    }
};

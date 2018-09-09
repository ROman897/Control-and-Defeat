#pragma once
#include <unordered_map>
#include <string>

#include <glad/glad.h>

struct Texture2D {
    GLuint m_texture_id;
};

class ResourceManager {
private:
    std::unordered_map<std::string, GLuint> m_loaded_sprites;

    Texture2D load_texture(const std::string& filename);
};

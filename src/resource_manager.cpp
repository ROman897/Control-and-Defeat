#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "resource_manager.hpp"

Texture2D ResourceManager::load_texture(const std::string& filename) {
    auto found_sprite = m_loaded_sprites.find(filename);
    if (found_sprite != m_loaded_sprites.end()) {
        return {found_sprite->second};
    }
    int width, height, channels;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    assert(data);
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    m_loaded_sprites[filename] = texture_id;
    return {texture_id};
}

#include "texturemanager.hpp"

std::map<const std::string, sf::Texture *> TextureManager::textures_;

sf::Texture *TextureManager::GetTexture(const std::string name) {
  return textures_.at(name);
}

TextureManager::~TextureManager() {}
#include "texturemanager.hpp"

std::map<const std::string, sf::Texture *> TextureManager::textures_;

sf::Texture *TextureManager::GetTexture(const std::string name) {
  if (textures_.find(name) != textures_.end()) {
    return textures_.at(name);
  } else {
    LoadTexture(name);
  }
  return textures_.at(name);
}

sf::Texture *TextureManager::LoadTexture(const std::string name) {
  sf::Texture *texture = new sf::Texture();
  texture->loadFromFile(name);
  textures_[name] = texture;
  return textures_[name];
}
TextureManager::~TextureManager() {}
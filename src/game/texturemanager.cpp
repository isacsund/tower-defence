#include "texturemanager.hpp"
#include <iostream>

std::map<const std::string, std::shared_ptr<sf::Texture>>
    TextureManager::textures_;

std::shared_ptr<sf::Texture> TextureManager::GetTexture(
    const std::string name) {
  if (textures_.find(name) != textures_.end()) {
    return textures_.at(name);
  } else {
    LoadTexture(name);
  }
  return textures_.at(name);
}

std::shared_ptr<sf::Texture> TextureManager::LoadTexture(
    const std::string name) {
  std::shared_ptr<sf::Texture> texture =
      std::make_shared<sf::Texture>(sf::Texture());
  texture->loadFromFile(name);
  textures_[name] = texture;
  return textures_[name];
}
TextureManager::~TextureManager() {
  for (auto it = textures_.begin(); it != textures_.end(); it++) {
    delete it->second.get();
  }
}
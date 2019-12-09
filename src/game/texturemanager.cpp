#include "texturemanager.hpp"
#include <iostream>
TextureManager& TextureManager::GetInstance() {
  static TextureManager instance;
  return instance;
}

sf::Texture& TextureManager::GetTexture(const std::string name) {
  auto texture = textures_.find(name);
  // If the texture doesn't exist we load it first
  if (texture == textures_.end()) {
    texture = LoadTexture(name);
  }
  return *texture->second;
}

std::map<const std::string, std::unique_ptr<sf::Texture>>::iterator
TextureManager::LoadTexture(const std::string name) {
  std::unique_ptr<sf::Texture> texture =
      std::make_unique<sf::Texture>(sf::Texture());
  texture->loadFromFile(name);
  return textures_.insert(std::make_pair(name, std::move(texture))).first;
}
#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <map>
#include <memory>

class TextureManager {
 public:
  static TextureManager& GetInstance();
  sf::Texture& GetTexture(const std::string name);
  std::map<const std::string, std::unique_ptr<sf::Texture>>::iterator
  LoadTexture(const std::string name);

  TextureManager(TextureManager const&) = delete;
  void operator=(TextureManager const&) = delete;

 private:
  TextureManager() {}
  std::map<const std::string, std::unique_ptr<sf::Texture>> textures_;
};

#define texture_manager TextureManager::GetInstance()
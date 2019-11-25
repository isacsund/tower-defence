#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <map>
#include <memory>

class TextureManager {
 public:
  ~TextureManager();

  static std::shared_ptr<sf::Texture> GetTexture(const std::string name);
  static std::shared_ptr<sf::Texture> LoadTexture(const std::string name);

 private:
  static std::map<const std::string, std::shared_ptr<sf::Texture>> textures_;
  TextureManager(){};
};
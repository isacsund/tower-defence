#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <map>

class TextureManager {
 public:
  ~TextureManager();

  static sf::Texture *GetTexture(const std::string name);
  static sf::Texture *LoadTexture(const std::string name);

 private:
  static std::map<const std::string, sf::Texture *> textures_;
  TextureManager(){};
};
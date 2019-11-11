#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "../enemy/enemy.hpp"
#include "../map/map.hpp"

class Game {
 public:
  Game();
  void Run();
  void DrawAll();
  void DrawMap();
  void DrawEnemies();
  void LoadTextures();
  void LoadTexture(const std::string texture_name);

 private:
  Map map_;
  sf::RenderWindow window_;
  sf::View view_;
  std::map<const std::string, sf::Texture> textures_;
};
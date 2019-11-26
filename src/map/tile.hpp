#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>

enum TileTypes { Path, Empty, EnemySpawn, PlayerBase };

class Tile {
 public:
  Tile(TileTypes type = Empty);
  TileTypes GetType() const;
  sf::Texture& GetTexture() const;
  sf::Sprite* GetSprite();

 private:
  TileTypes type_;
  std::string texturename_;
  sf::Sprite sprite_;
};

bool IsTraversable(TileTypes type);

std::ostream& operator<<(std::ostream& os, const Tile& tile);

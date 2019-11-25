#pragma once
#include <iostream>
#include "../game/texturemanager.hpp"
#include "SFML/Graphics.hpp"

enum TileTypes { Path, Empty, EnemySpawn, PlayerBase };

class Tile {
 public:
  Tile(TileTypes type = Empty);
  TileTypes GetType() const;
  const std::string& GetTexture() const;
  sf::Sprite* GetSprite();

 private:
  TileTypes type_;
  std::string texture_;
  sf::Sprite sprite_;
};

bool IsTraversable(TileTypes type);

std::ostream& operator<<(std::ostream& os, const Tile& tile);

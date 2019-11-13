#pragma once
#include <iostream>

enum TileTypes { Path, Empty, EnemySpawn, PlayerBase };

class Tile {
 public:
  Tile(TileTypes type = Empty);
  const TileTypes GetType() const;
  const std::string& GetTexture() const;

 private:
  TileTypes type_;
  std::string texture_;
};

bool IsTraversable(TileTypes type);

std::ostream& operator<<(std::ostream& os, const Tile& tile);

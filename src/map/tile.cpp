#include "tile.hpp"

Tile::Tile(TileTypes type) : type_(type) {
  switch (type) {
    case Path:
      texture_ = "sprites/sand_tile.png";
      break;
    case PlayerBase:
      texture_ = "sprites/sand_tile.png";
      break;
    case EnemySpawn:
      texture_ = "sprites/sand_tile.png";
      break;
    default:
      texture_ = "sprites/grass_tile_1.png";
  }
}

TileTypes Tile::GetType() const { return type_; }

const std::string& Tile::GetTexture() const { return texture_; }

bool IsTraversable(TileTypes type) {
  switch (type) {
    case Path:
    case EnemySpawn:
    case PlayerBase:
      return true;
    case Empty:
    default:
      return false;
  }
}

std::ostream& operator<<(std::ostream& os, const Tile& tile) {
  os << tile.GetType();
  return os;
};
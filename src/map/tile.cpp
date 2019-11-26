#include "tile.hpp"
#include "../game/texturemanager.hpp"

Tile::Tile(TileTypes type) : type_(type) {
  switch (type) {
    case Path:
      texturename_ = "sprites/sand_tile.png";
      break;
    case PlayerBase:
      texturename_ = "sprites/sand_tile.png";
      break;
    case EnemySpawn:
      texturename_ = "sprites/sand_tile.png";
      break;
    default:
      texturename_ = "sprites/grass_tile_1.png";
  }
  sprite_ = sf::Sprite(GetTexture());
}

TileTypes Tile::GetType() const { return type_; }

sf::Texture& Tile::GetTexture() const {
  return texture_manager.GetTexture(texturename_);
}

sf::Sprite* Tile::GetSprite() { return &sprite_; }

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
}
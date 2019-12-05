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
    case Tree1:
      texturename_ = "sprites/tree_1.png";
      break;
    case Tree2:
      texturename_ = "sprites/tree_2.png";
      break;
    case Tree3:
      texturename_ = "sprites/tree_3.png";
      break;
    case EnemySpawn:
      texturename_ = "sprites/sand_tile.png";
      break;
    case Water1:
      texturename_ = "sprites/water_1.png";
      break;
    case Water2:
      texturename_ = "sprites/water_2.png";
      break;
    case Water3:
      texturename_ = "sprites/water_3.png";
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

void Tile::SetPosition(float x, float y) { sprite_.setPosition(x, y); }
void Tile::SetScale(float tile_size) {
  sprite_.setScale(tile_size / (float)(*sprite_.getTexture()).getSize().x,
                   tile_size / (float)(*sprite_.getTexture()).getSize().y);
}

void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(sprite_, states);
}
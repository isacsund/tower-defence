#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

enum TileTypes {
  Path,
  Empty,
  Tree1,
  Tree2,
  Tree3,
  EnemySpawn,
  PlayerBase,
  Water1,
  Water2,
  Water3
};

class Tile : public sf::Drawable {
 public:
  Tile(TileTypes type = Empty);
  TileTypes GetType() const;
  sf::Texture& GetTexture() const;
  sf::Sprite* GetSprite();
  void SetPosition(float x, float y);
  void SetScale(float tile_size);

 private:
  TileTypes type_;
  std::string texturename_;
  sf::Sprite sprite_;
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

bool IsTraversable(TileTypes type);

std::ostream& operator<<(std::ostream& os, const Tile& tile);

#include "tower.hpp"

Tower::Tower(float range, float damage, int x, int y, TowerTypes type,
             const std::string& texture)
    : range_(range),
      damage_(damage),
      x_(x),
      y_(y),
      texture_(texture),
      type_(type) {}

const std::pair<int, int> Tower::GetPosition() const { return {x_, y_}; }
const std::string& Tower::GetTexture() const { return texture_; }
void Tower::Attack() const {
  // search for enemy
  // iterate enemy collection, select enemy farthest on map within tower range
  // attack enemy
}
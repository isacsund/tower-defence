#include "basic_tower.hpp"

BasicTower::BasicTower(float range, float damage, float att_speed, int x, int y,
                       float size, int price, const std::string& texturename)
    : Tower(range, damage, att_speed, x, y, size, price, texturename) {
  max_upgrade_ = 3;
}

void BasicTower::Upgrade() {
  if (current_upgrade_ < max_upgrade_) {
    current_upgrade_ += 1;
    switch (current_upgrade_) {
      case 2:
        damage_ += 2;
        break;
      case 3:
        range_ += 1;
        break;
      default:
        break;
    }
  }
}
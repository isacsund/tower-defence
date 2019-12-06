#include "ship_tower.hpp"

ShipTower::ShipTower(float range, float damage, float att_speed, int x, int y,
                     float size, int price, const std::string& texturename)
    : Tower(range, damage, att_speed, x, y, size, price, texturename) {
  max_upgrade_ = 3;
}

void ShipTower::Upgrade() {
  if (current_upgrade_ < max_upgrade_) {
    current_upgrade_ += 1;
    switch (current_upgrade_) {
      case 2:
        damage_ += 5;
        break;
      case 3:
        range_ += 2;
        break;
      default:
        break;
    }
  }
}
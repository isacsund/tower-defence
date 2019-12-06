#include "money_tower.hpp"

MoneyTower::MoneyTower(int x, int y, float size, int price,
                       const std::string& texturename, int money_per_wave)
    : Tower(0, 0, 0, x, y, size, price, texturename),
      money_per_wave_(money_per_wave) {
  max_upgrade_ = 3;
}

void MoneyTower::Upgrade() {
  if (current_upgrade_ < max_upgrade_) {
    current_upgrade_ += 1;
    money_per_wave_ += 300;
  }
}
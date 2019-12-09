#include "money_tower.hpp"

MoneyTower::MoneyTower(int x, int y, float size, int price,
                       const std::string& texturename)
    : Tower(0, 0, 0, x, y, size, price, texturename) {
  max_upgrade_ = 4;
  upgrade_price_ = 100;
  money_per_wave_ = 100;
}

void MoneyTower::Upgrade() {
  if (current_upgrade_ < max_upgrade_) {
    current_upgrade_ += 1;
    switch (current_upgrade_) {
      case 2:
        money_per_wave_ += 50;
        upgrade_price_ += 50;
        break;
      case 3:
        money_per_wave_ += 50;
        upgrade_price_ += 50;
        break;
      case 4:
        money_per_wave_ += 50;
        upgrade_price_ += 100;
        break;
      default:
        break;
    }
  }
}
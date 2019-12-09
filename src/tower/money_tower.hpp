#include "tower.hpp"

class MoneyTower : public Tower {
 public:
  MoneyTower(int x, int y, float size, int price,
             const std::string& texturename = "sprites/money_tower.png");
  void Upgrade();
};

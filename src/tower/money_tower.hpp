#include "tower.hpp"

class MoneyTower : public Tower {
 public:
  MoneyTower(int x, int y, float size, int price,
             const std::string& texturename = "sprites/money_tower.png",
             int money_per_wave = 300);
  void Upgrade();

 private:
  int money_per_wave_;
};

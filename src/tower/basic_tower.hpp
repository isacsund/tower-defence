#include "tower.hpp"

class BasicTower : public Tower {
 public:
  BasicTower(float range, float damage, float att_speed, int x, int y,
             float size, int price,
             const std::string& texturename = "sprites/basic_tower.png");

  void Upgrade();
};
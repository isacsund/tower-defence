#include "tower.hpp"

class ShipTower : public Tower {
 public:
  ShipTower(float range, float damage, float att_speed, int x, int y,
            float size, int price,
            const std::string& texturename = "sprites/ship_tower.png");
};
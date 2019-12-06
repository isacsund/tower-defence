#include "ship_tower.hpp"

ShipTower::ShipTower(float range, float damage, float att_speed, int x, int y,
                     float size, int price, const std::string& texturename)
    : Tower(range, damage, att_speed, x, y, size, price, texturename) {}
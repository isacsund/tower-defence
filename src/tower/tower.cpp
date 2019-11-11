#include "tower.hpp"

Tower::Tower(float range, float damage, const std::string texture,
             TowerTypes type)
    : range_(range), damage_(damage), texture_(texture), type_(type) {}
void Tower::Attack() const {
  // search for enemy
  /*interate enemy collection, select enemy fartest on map*/
  // attack enemy
}
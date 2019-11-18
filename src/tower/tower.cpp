#include "tower.hpp"

Tower::Tower(float range, float damage, float att_speed, int x, int y,
             TowerTypes type, const std::string& texture)
    : range_(range),
      damage_(damage),
      att_speed_(att_speed),
      x_(x),
      y_(y),
      texture_(texture),
      type_(type) {}

const std::pair<int, int> Tower::GetPosition() const { return {x_, y_}; }
const std::string& Tower::GetTexture() const { return texture_; }
float Tower::GetRange() const { return range_; }
void Tower::Attack(Enemy& enemy) const { enemy.SetHp(enemy.GetHp() - damage_); }
float Tower::GetAttSpeed() const { return att_speed_; }
float Tower::GetLastAttack() const { return last_attack_; }
void Tower::SetLastAttack(float att_time) { last_attack_ = att_time; }
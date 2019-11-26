#include "tower.hpp"
#include "../game/texturemanager.hpp"

Tower::Tower(float range, float damage, float att_speed, int x, int y,
             TowerTypes type, const std::string& texturename)
    : range_(range),
      damage_(damage),
      att_speed_(att_speed),
      x_(x),
      y_(y),
      type_(type),
      texturename_(texturename),
      last_attack_(0) {
  sprite_ = sf::Sprite(GetTexture());
}

const std::pair<int, int> Tower::GetPosition() const { return {x_, y_}; }
float Tower::GetRange() const { return range_; }
void Tower::Attack(Enemy& enemy) const { enemy.SetHp(enemy.GetHp() - damage_); }
float Tower::GetAttSpeed() const { return att_speed_; }
float Tower::GetLastAttack() const { return last_attack_; }
void Tower::SetLastAttack(float att_time) { last_attack_ = att_time; }
sf::Texture& Tower::GetTexture() const {
  return texture_manager.GetTexture(texturename_);
}
sf::Sprite* Tower::GetSprite() { return &sprite_; }
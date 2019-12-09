#include "tower.hpp"
#include <iostream>
#include "../game/texturemanager.hpp"

Tower::Tower(float range, float damage, float att_speed, int x, int y,
             float size, int price, const std::string& texturename)
    : range_(range),
      damage_(damage),
      current_upgrade_(1),
      att_speed_(att_speed),
      money_per_wave_(0),
      x_(x),
      y_(y),
      size_(size),
      price_(price),
      texturename_(texturename),
      last_attack_(0) {
  sprite_ = sf::Sprite(GetTexture());
  sprite_.setScale(size / (float)(*sprite_.getTexture()).getSize().x,
                   size / (float)(*sprite_.getTexture()).getSize().y);
  radius_.setRadius(range);
  radius_.setFillColor(sf::Color(255, 255, 255, 100));
}

const std::pair<int, int> Tower::GetPosition() const { return {x_, y_}; }
float Tower::GetRange() const { return range_; }
bool Tower::Attack(Enemy& enemy) const {
  enemy.SetHp(enemy.GetHp() - damage_);
  return !enemy.IsAlive();
}
float Tower::GetAttSpeed() const { return att_speed_; }
float Tower::GetDamage() const { return damage_; }
float Tower::GetLastAttack() const { return last_attack_; }
void Tower::SetLastAttack(float att_time) { last_attack_ = att_time; }
sf::Texture& Tower::GetTexture() const {
  return texture_manager.GetTexture(texturename_);
}
sf::Sprite* Tower::GetSprite() { return &sprite_; }

void Tower::SetPosition(float x, float y) {
  sprite_.setPosition(x, y);

  // radius_.setPosition(x - range_ + size_ / 2, y - range_ + size_ / 2);
}

void Tower::SetScale(float factor_x, float factor_y) {
  sprite_.setScale(factor_x, factor_y);
  auto tile_size = factor_x * GetTexture().getSize().x;
  float radius = tile_size * range_;
  radius_.setRadius(radius);
  radius_.setPosition(
      sprite_.getPosition() +
      sf::Vector2f(-radius + tile_size / 2, -radius + tile_size / 2));
}

void Tower::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  if (active_) target.draw(radius_);
  target.draw(sprite_, states);
}

void Tower::SetActive() { active_ = true; }
void Tower::SetInactive() { active_ = false; }
bool Tower::IsActive() const { return active_; }
int Tower::GetPrice() const { return price_; }
int Tower::GetCurrentUpgrade() const { return current_upgrade_; }
int Tower::GetUpgradePrice() const { return upgrade_price_; }
bool Tower::IsUpgradeable() const { return (current_upgrade_ < max_upgrade_); }
int Tower::GetMoneyPerWave() const { return money_per_wave_; }
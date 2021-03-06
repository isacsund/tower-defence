#include "enemy.hpp"
#include <math.h>
#include <algorithm>
#include <iostream>
#include "../game/texturemanager.hpp"

Enemy::Enemy(float max_hp, float speed, float x, float y, float size,
             float delay, EnemyTypes type)
    : max_hp_(max_hp),
      hp_(max_hp),
      speed_(speed),
      x_(x),
      y_(y),
      delay_(delay),
      type_(type),
      target_tile_({-1, -1}) {
  switch (type) {
    case Fast:
      texture_name_ = "sprites/enemy_2.png";
      break;
    case Big:
      texture_name_ = "sprites/enemy_3.png";
      break;
    case Magic:
      texture_name_ = "sprites/enemy_4.png";
      break;
    case Boss:
      texture_name_ = "sprites/enemy_5.png";
      break;
    default:
      texture_name_ = "sprites/enemy_1.png";
  }

  sprite_ = sf::Sprite(GetTexture());
  sprite_.setScale(size / (float)(*sprite_.getTexture()).getSize().x,
                   size / (float)(*sprite_.getTexture()).getSize().y);
  hp_bar_green_.setFillColor(sf::Color::Green);
  hp_bar_red_.setFillColor(sf::Color::Red);

  float hp_ratio = hp_ / max_hp_;

  hp_bar_red_.setSize(sf::Vector2f(HP_BAR_WIDTH, HP_BAR_HEIGHT));
  hp_bar_green_.setSize(sf::Vector2f(HP_BAR_WIDTH * hp_ratio, HP_BAR_HEIGHT));
}

void Enemy::Move(const std::vector<std::pair<int, int>>& path) {
  if (!IsAlive()) return;
  if (target_tile_ == GetTile() ||
      (target_tile_.first == -1 && target_tile_.second == -1)) {
    target_tile_ = FindNextTile(path);
  }
  float target_x = ((target_tile_.first + 0.5) + ((int)x_ + 0.5)) / 2;
  float target_y = ((target_tile_.second + 0.5) + ((int)y_ + 0.5)) / 2;
  float dx = target_x - x_;
  float dy = target_y - y_;
  float dist = sqrt(pow(dx, 2) + pow(dy, 2));
  if (dist != 0) {
    dx /= dist;
    dy /= dist;
  }
  x_ += dx * speed_ / 100;
  y_ += dy * speed_ / 100;
}

float Enemy::GetHp() const { return hp_; }
float Enemy::GetMaxHp() const { return max_hp_; }
float Enemy::GetSpeed() const { return speed_; }
float Enemy::GetDelay() const { return delay_; }
const std::pair<float, float> Enemy::GetPosition() const { return {x_, y_}; }
const std::pair<int, int> Enemy::GetTile() const { return {int(x_), int(y_)}; }
EnemyTypes Enemy::GetType() const { return type_; }
bool Enemy::IsAlive() const { return hp_ > 0; }
void Enemy::SetHp(float hp) { hp_ = hp; }

const std::pair<int, int> Enemy::FindNextTile(
    const std::vector<std::pair<int, int>>& path) const {
  auto target_tile = path[0];
  if (target_tile_.first == -1 && target_tile_.second == -1) {
    return target_tile;
  }
  auto it = std::find(path.begin(), path.end(), target_tile_);
  if (it != path.end()) {
    size_t idx = std::distance(path.begin(), it);
    if (idx < path.size() - 1) {
      target_tile = path[idx + 1];
    }
    return target_tile;
  } else {
    std::cout << "Error finding next tile on path" << std::endl;
  }
  return target_tile;
}

void Enemy::SetPosition(float x, float y) {
  sprite_.setPosition(x, y);
  auto hp_bar_offset = hp_bar_red_.getSize().x;
  hp_bar_green_.setPosition(x + hp_bar_offset / 2, y);
  hp_bar_red_.setPosition(x + hp_bar_offset / 2, y);
}

void Enemy::SetScale(float factor_x, float factor_y) {
  sprite_.setScale(factor_x, factor_y);
  float hp_ratio = hp_ / max_hp_;

  auto sprite_size = sprite_.getLocalBounds();
  auto sprite_width = sprite_size.width * factor_x;

  hp_bar_red_.setSize(sf::Vector2f(sprite_width / 2, sprite_width / 10));

  hp_bar_green_.setSize(
      sf::Vector2f(sprite_width / 2 * hp_ratio, sprite_width / 10));
}

sf::Texture& Enemy::GetTexture() const {
  return texture_manager.GetTexture(texture_name_);
}

sf::Sprite* Enemy::GetSprite() { return &sprite_; }

// Debugging function
std::ostream& operator<<(std::ostream& os, const Enemy& enemy) {
  os << "Enemy at: (" << enemy.GetPosition().first << ", "
     << enemy.GetPosition().second << ") with " << enemy.GetHp() << "/"
     << enemy.GetMaxHp() << " hp";
  return os;
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(sprite_, states);
  target.draw(hp_bar_red_, states);
  target.draw(hp_bar_green_, states);
}

Enemy& Enemy::operator=(const Enemy& enemy) {
  max_hp_ = enemy.max_hp_;
  speed_ = enemy.speed_;
  x_ = enemy.x_;
  y_ = enemy.y_;
  size_ = enemy.size_;
  delay_ = enemy.delay_;
  sprite_ = enemy.sprite_;
  type_ = enemy.type_;
  target_tile_ = enemy.target_tile_;
  texture_name_ = enemy.texture_name_;
  hp_bar_green_ = enemy.hp_bar_green_;
  hp_bar_red_ = enemy.hp_bar_red_;
  hp_ = enemy.hp_;
  return *this;
}

#include "enemy.hpp"
#include <math.h>
#include <algorithm>
#include <iostream>
#include "../game/texturemanager.hpp"

Enemy::Enemy(float max_hp, float speed, float x, float y,
             const std::string& texture_name, EnemyTypes type)
    : max_hp_(max_hp),
      hp_(max_hp),
      speed_(speed),
      x_(x),
      y_(y),
      texture_name_(texture_name),
      type_(type),
      target_tile_({-1, -1}) {
  sprite_ = sf::Sprite(GetTexture());
}

void Enemy::Move(const std::vector<std::pair<int, int>>& path) {
  if (!IsAlive()) return;
  if (target_tile_ == GetTile() ||
      (target_tile_.first == -1 && target_tile_.second == -1)) {
    target_tile_ = FindNextTile(path);
  }
  float target_x = target_tile_.first + 0.5;
  float target_y = target_tile_.second + 0.5;
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

/*
void Enemy::Draw(sf::RenderWindow& window) {
  auto windowsize = window.getSize();
  int enemy_size_x = (windowsize.x - 200) / map.GetWidth();
  int enemy_size_y = (windowsize.y) / map.GetHeight();
  auto enemy_size = std::min(enemy_size_x, enemy_size_y);

  sprite_.setPosition(x_ * enemy_size - enemy_size / 2,
                      y_ * enemy_size - enemy_size / 2);
  sprite_.setScale(enemy_size / (float)(*sprite_.getTexture()).getSize().x,
                   enemy_size / (float)(*sprite_.getTexture()).getSize().y);
  window.draw(sprite_);

  // Hp bar
  const int hp_bar_width = enemy_size / 2;
  const int hp_bar_height = enemy_size / 10;
  float hp_ratio = hp_ / max_hp_;
  sf::RectangleShape hp_bar_green;
  sf::RectangleShape hp_bar_red;

  hp_bar_green.setFillColor(sf::Color::Green);
  hp_bar_red.setFillColor(sf::Color::Red);
  hp_bar_red.setSize(sf::Vector2f(hp_bar_width, hp_bar_height));
  hp_bar_green.setSize(sf::Vector2f(hp_bar_width * hp_ratio, hp_bar_height));
  hp_bar_green.setPosition(x_ * enemy_size - hp_bar_width / 2,
                           y_ * enemy_size - enemy_size / 2);
  hp_bar_red.setPosition(hp_bar_green.getPosition());
  window.draw(hp_bar_red);
  window.draw(hp_bar_green);
}
*/
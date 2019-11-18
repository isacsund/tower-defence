#include "enemy.hpp"
#include <math.h>
#include <algorithm>
#include <iostream>

Enemy::Enemy(float hp, float speed, float x, float y,
             const std::string& texture, EnemyTypes type)
    : hp_(hp),
      speed_(speed),
      x_(x),
      y_(y),
      texture_(texture),
      type_(type),
      target_tile_({-1, -1}) {}

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

int Enemy::GetHp() const { return hp_; }
int Enemy::GetSpeed() const { return speed_; }
const std::pair<float, float> Enemy::GetPosition() const { return {x_, y_}; }
const std::pair<int, int> Enemy::GetTile() const { return {int(x_), int(y_)}; }
const std::string& Enemy::GetTexture() const { return texture_; }
const EnemyTypes Enemy::GetType() const { return type_; }
bool Enemy::IsAlive() const { return hp_ > 0; }
void Enemy::SetHp(int hp) { hp_ = hp; }

const std::pair<int, int> Enemy::FindNextTile(
    const std::vector<std::pair<int, int>>& path) const {
  auto target_tile = path[0];
  if (target_tile_.first == -1 && target_tile_.second == -1) {
    return target_tile;
  }
  auto it = std::find(path.begin(), path.end(), target_tile_);
  if (it != path.end()) {
    int idx = std::distance(path.begin(), it);
    if (idx < path.size() - 1) {
      target_tile = path[idx + 1];
    }
    return target_tile;
  } else {
    std::cout << "Error finding next tile on path" << std::endl;
  }
  return target_tile;
}

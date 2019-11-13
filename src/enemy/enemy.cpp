#include "enemy.hpp"

Enemy::Enemy(int hp, int speed, int x, int y, const std::string& texture,
             EnemyTypes type)
    : hp_(hp), speed_(speed), x_(x), y_(y), texture_(texture), type_(type) {}

void Enemy::Move() {}  // TODO: Implement A* search algorithm xd
int Enemy::GetHp() const { return hp_; }
int Enemy::GetSpeed() const { return speed_; }
std::pair<int, int> Enemy::GetLocation() const { return {x_, y_}; }
const std::string& Enemy::GetTexture() const { return texture_; }
const EnemyTypes Enemy::GetType() const { return type_; }

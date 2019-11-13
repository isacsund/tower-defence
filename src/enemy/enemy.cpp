#include "enemy.hpp"

Enemy::Enemy(int hp, int speed, int x, int y, const std::string& texture,
             EnemyTypes type)
    : hp_(hp), speed_(speed), x_(x), y_(y), texture_(texture), type_(type) {}

void Enemy::Move() {}  // TODO: Implement A* search algorithm xd
const int Enemy::GetHp() const { return hp_; }
const int Enemy::GetSpeed() const { return speed_; }
const std::pair<int, int> Enemy::GetPosition() const { return {x_, y_}; }
const std::string& Enemy::GetTexture() const { return texture_; }
const EnemyTypes Enemy::GetType() const { return type_; }
const bool Enemy::IsAlive() const { return hp_ > 0; }
void Enemy::SetHp(int hp) { hp_ = hp; }
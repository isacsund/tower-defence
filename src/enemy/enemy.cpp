#include "enemy.hpp"

Enemy::Enemy(int hp, int speed, float x, float y, const std::string& texture,
             EnemyTypes type)
    : hp_(hp), speed_(speed), x_(x), y_(y), texture_(texture), type_(type) {}

void Enemy::Move() {}
int Enemy::GetHp() const { return hp_; }
int Enemy::GetSpeed() const { return speed_; }
std::pair<int, int> Enemy::GetLocation() const { return {x_, y_}; }
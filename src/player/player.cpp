#include "player.hpp"

Player::Player(std::string name, int money, int score)
    : money_(money), score_(score), name_(name) {}
int Player::GetMoney() const { return money_; }
int Player::GetScore() const { return score_; }
std::string Player::GetName() const { return name_; }
void Player::AddMoney(int money) { money_ += money; }
void Player::AddScore(int score) { score_ += score; }
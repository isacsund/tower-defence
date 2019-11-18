#include "player.hpp"

Player::Player(int money, int score) : money_(money), score_(score) {}
int Player::GetMoney() const { return money_; }
int Player::GetScore() const { return score_; }
void Player::AddMoney(int money) { money_ += money; }
void Player::AddScore(int score) { score_ += score; }
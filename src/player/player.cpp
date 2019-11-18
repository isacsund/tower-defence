#include "player.hpp"

Player::Player(const std::string& name, int lives = 5, int money, int score)
    : money_(money), score_(score), name_(name), lives_(lives) {}
int Player::GetMoney() const { return money_; }
int Player::GetScore() const { return score_; }
int Player::GetLives() const { return lives_; }
const std::string& Player::GetName() const { return name_; }
void Player::AddMoney(int money) { money_ += money; }
void Player::AddScore(int score) { score_ += score; }
void Player::RemoveLives(int lives) { lives_ -= lives; }
#include "player.hpp"

Player::Player(const std::string& name, int lives, int money)
    : name_(name), lives_(lives), money_(money) {}
int Player::GetMoney() const { return money_; }
int Player::GetLives() const { return lives_; }
const std::string& Player::GetName() const { return name_; }
void Player::AddMoney(int money) { money_ += money; }
void Player::RemoveLives(int lives) { lives_ -= lives; }

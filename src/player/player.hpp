#include <string>

class Player {
 public:
  Player(std::string name, int money = 500, int score = 0);
  int GetMoney() const;
  int GetScore() const;
  std::string GetName() const;
  void AddMoney(int money);
  void AddScore(int score);

 private:
  int money_;
  int score_;
  std::string name_;
};

#include <string>

class Player {
 public:
  Player(const std::string& name, int money = 500, int score = 0);
  int GetMoney() const;
  int GetScore() const;
  const std::string& GetName() const;
  void AddMoney(int money);
  void AddScore(int score);

 private:
  int money_;
  int score_;
  const std::string name_;
};

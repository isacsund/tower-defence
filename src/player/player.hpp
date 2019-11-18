#include <string>

class Player {
 public:
  Player(const std::string& name, int lives = 10, int money = 500,
         int score = 0);
  int GetMoney() const;
  int GetScore() const;
  int GetLives() const;
  const std::string& GetName() const;
  void AddMoney(int money);
  void AddScore(int score);
  void RemoveLives(int damage);

 private:
  int money_;
  int score_;
  int lives_;
  const std::string name_;
};

#include <string>

class Player {
 public:
  Player(const std::string& name, int lives, int money);
  int GetMoney() const;
  int GetLives() const;
  const std::string& GetName() const;
  void AddMoney(int money);
  void RemoveLives(int damage);

 private:
  const std::string name_;
  int lives_;
  int money_;
};

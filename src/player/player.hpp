

class Player {
 public:
  Player(int money = 500, int score = 0);
  int GetMoney() const;
  int GetScore() const;
  void AddMoney(int money);
  void AddScore(int score);

 private:
  int money_;
  int score_;
};

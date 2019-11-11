#include <string>

enum EnemyTypes { Standard, Fast /*, Big, Boss*/ };

class Enemy {
 public:
  Enemy(int hp, int speed, float x, float y,
        const std::string& texture = "sprites/basic_enemy.png",
        EnemyTypes type = Standard);
  void Move();
  int GetHp() const;
  int GetSpeed() const;
  std::pair<int, int> GetLocation() const;
  const std::string& GetTexture() const;
  const EnemyTypes GetType() const;

 protected:
  int hp_;

 private:
  int speed_;
  float x_, y_;
  const std::string texture_;
  EnemyTypes type_;
};
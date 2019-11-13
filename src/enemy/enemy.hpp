#include <string>

enum EnemyTypes { Standard, Fast /*, Big, Boss*/ };

class Enemy {
 public:
  Enemy(int hp, int speed, float x, float y,
        const std::string& texture = "sprites/basic_enemy.png",
        EnemyTypes type = Standard);
  void Move();
  const int GetHp() const;
  const int GetSpeed() const;
  const std::pair<float, float> GetPosition() const;
  const std::string& GetTexture() const;
  const EnemyTypes GetType() const;
  const bool IsAlive() const;
  void SetHp(int hp);

 private:
  int hp_;
  int speed_;
  float x_, y_;
  const std::string texture_;
  EnemyTypes type_;
};
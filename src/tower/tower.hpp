#include <string>
#include "../enemy/enemy.hpp"

enum TowerTypes {
  Basic,
  Rapid /*, Sniper, Special*/
};

class Tower {
 public:
  Tower(float range, float damage, float att_speed, int x, int y,
        TowerTypes type = Basic,
        const std::string& texture = "sprites/basic_tower.png");
  void Attack(Enemy& enemy) const;
  const std::pair<int, int> GetPosition() const;
  const std::string& GetTexture() const;
  const float GetRange() const;
  float GetAttSpeed() const;
  float GetLastAttack() const;
  void SetLastAttack(float att_time);

 private:
  float range_;
  float damage_;
  float att_speed_;
  int x_, y_;
  const std::string texture_;
  TowerTypes type_;
  float last_attack_;
};
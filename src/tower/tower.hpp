#include <string>

enum TowerTypes {
  Basic,
  Rapid /*, Sniper, Special*/
};

class Tower {
 public:
  Tower(float range, float damage, int x, int y, TowerTypes type = Basic,
        const std::string& texture = "sprites/basic_tower.png");
  void Attack() const;
  std::pair<int, int> GetLocation() const;
  const std::string& GetTexture() const;

 private:
  float range_;
  float damage_;
  int x_, y_;
  const std::string texture_;
  TowerTypes type_;
};
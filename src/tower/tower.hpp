#include <string>

enum TowerTypes {
  Standard,
  Rapid /*, Sniper, Special*/
};

class Tower {
 public:
  Tower(float range, float damage, const std::string texture, TowerTypes type);
  void Attack() const;

 private:
  float range_;
  float damage_;
  const std::string texture_;
  TowerTypes type_;
};
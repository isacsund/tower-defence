#include <string>
#include "../enemy/enemy.hpp"
#include "SFML/Graphics.hpp"

enum TowerTypes {
  Basic,
  Rapid /*, Sniper, Special*/
};

class Tower {
 public:
  Tower(float range, float damage, float att_speed, int x, int y,
        const std::string& texturename = "sprites/basic_tower.png",
        TowerTypes type = Basic);
  void Attack(Enemy& enemy) const;
  const std::pair<int, int> GetPosition() const;

  float GetRange() const;
  float GetAttSpeed() const;
  float GetLastAttack() const;
  void SetLastAttack(float att_time);
  sf::Texture& GetTexture() const;
  sf::Sprite* GetSprite();

 private:
  float range_;
  float damage_;
  float att_speed_;
  int x_, y_;
  TowerTypes type_;
  std::string texturename_;
  float last_attack_;
  sf::Sprite sprite_;
};
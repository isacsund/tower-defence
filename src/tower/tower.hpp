#pragma once
#include <string>
#include "../enemy/enemy.hpp"
#include "SFML/Graphics.hpp"

class Tower : public sf::Drawable {
 public:
  Tower(float range, float damage, float att_speed, int x, int y, float size,
        int price, const std::string& texturename = "sprites/basic_tower.png");
  bool Attack(Enemy& enemy) const;
  const std::pair<int, int> GetPosition() const;

  float GetRange() const;
  float GetAttSpeed() const;
  float GetDamage() const;
  float GetLastAttack() const;
  void SetLastAttack(float att_time);
  sf::Texture& GetTexture() const;
  sf::Sprite* GetSprite();
  void SetPosition(float x, float y);
  void SetScale(float factor_x, float factor_y);
  void SetActive();
  void SetInactive();
  bool IsActive() const;
  int GetPrice() const;
  int GetCurrentUpgrade() const;
  bool IsUpgradeable() const;
  virtual void Upgrade() = 0;
  virtual ~Tower(){};

 protected:
  int max_upgrade_;
  float range_;
  float damage_;
  int current_upgrade_;
  float att_speed_;

 private:
  int x_, y_;
  float size_;
  int price_;
  std::string texturename_;
  float last_attack_;
  sf::Sprite sprite_;
  sf::CircleShape radius_;
  bool active_;
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
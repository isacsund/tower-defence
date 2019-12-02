#pragma once
#include <string>
#include <vector>
#include "SFML/Graphics.hpp"

enum EnemyTypes { Standard, Fast /*, Big, Boss*/ };

class Enemy : public sf::Drawable {
 public:
  Enemy(float max_hp, float speed, float x, float y,
        const std::string& texturename = "sprites/basic_enemy.png",
        EnemyTypes type = Standard);
  void Move(const std::vector<std::pair<int, int>>& path);
  float GetHp() const;
  float GetMaxHp() const;
  float GetSpeed() const;
  const std::pair<float, float> GetPosition() const;
  const std::pair<int, int> GetTile() const;
  EnemyTypes GetType() const;
  bool IsAlive() const;
  void SetHp(float hp);
  const std::pair<int, int> FindNextTile(
      const std::vector<std::pair<int, int>>& path) const;
  sf::Texture& GetTexture() const;
  sf::Sprite* GetSprite();

 private:
  float max_hp_;
  float hp_;
  float speed_;
  float x_, y_;
  EnemyTypes type_;
  std::pair<int, int> target_tile_;
  const std::string texturename_;
  sf::Sprite sprite_;
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

std::ostream& operator<<(std::ostream& os, const Enemy& enemy);
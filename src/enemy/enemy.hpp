#pragma once
#include <string>
#include <vector>
#include "SFML/Graphics.hpp"

enum EnemyTypes { Standard, Fast, Big, Magic, Boss };

class Enemy : public sf::Drawable {
 public:
  Enemy(float max_hp, float speed, float x, float y, float size, int delay,
        EnemyTypes type = Standard);
  void Move(const std::vector<std::pair<int, int>>& path);
  float GetHp() const;
  float GetMaxHp() const;
  float GetSpeed() const;
  int GetDelay() const;
  const std::pair<float, float> GetPosition() const;
  const std::pair<int, int> GetTile() const;
  EnemyTypes GetType() const;
  bool IsAlive() const;
  void SetHp(float hp);
  const std::pair<int, int> FindNextTile(
      const std::vector<std::pair<int, int>>& path) const;
  sf::Texture& GetTexture() const;
  void SetPosition(float x, float y);
  void SetScale(float factor_x, float factor_y);
  sf::Sprite* GetSprite();

 private:
  float max_hp_;
  float hp_;
  float speed_;
  float x_, y_;
  int size_;
  int delay_;
  std::string texture_name_;
  EnemyTypes type_;
  std::pair<int, int> target_tile_;
  sf::Sprite sprite_;
  sf::RectangleShape hp_bar_green_;
  sf::RectangleShape hp_bar_red_;

  const int HP_BAR_WIDTH = size_ / 2;
  const int HP_BAR_HEIGHT = size_ / 10;

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

std::ostream& operator<<(std::ostream& os, const Enemy& enemy);
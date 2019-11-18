#pragma once
#include <string>
#include <vector>
enum EnemyTypes { Standard, Fast /*, Big, Boss*/ };

class Enemy {
 public:
  Enemy(float hp, float speed, float x, float y,
        const std::string& texture = "sprites/basic_enemy.png",
        EnemyTypes type = Standard);
  void Move(const std::vector<std::pair<int, int>>& path);
  const int GetHp() const;
  const int GetSpeed() const;
  const std::pair<float, float> GetPosition() const;
  const std::pair<int, int> GetTile() const;
  const std::string& GetTexture() const;
  const EnemyTypes GetType() const;
  const bool IsAlive() const;
  void SetHp(int hp);
  const std::pair<int, int> FindNextTile(
      const std::vector<std::pair<int, int>>& path) const;

 private:
  float hp_;
  float speed_;
  float x_, y_;
  const std::string texture_;
  EnemyTypes type_;
  std::pair<int, int> target_tile_;
};
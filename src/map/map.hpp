#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "../enemy/enemy.hpp"
#include "tile.hpp"

class Map {
 public:
  Map();
  void Load(const std::string& filename);
  void Draw(sf::RenderWindow& window);
  int GetWidth() const;
  int GetHeight() const;
  void SetName(const std::string& name);
  std::string GetName();
  const std::vector<std::vector<Tile>> GetTiles() const;
  const Tile& operator()(int x, int y);
  const std::pair<int, int> GetEnemySpawn() const;
  const std::pair<int, int> GetPlayerBase() const;
  bool RecalculatePath();
  std::vector<std::pair<int, int>> GetPath() const;
  std::vector<Enemy> LoadWave(int wave);
  int tile_size;

 private:
  std::string name_;
  std::vector<std::vector<Tile>> tiles_;

  std::pair<int, int> enemy_spawn_;
  std::pair<int, int> player_base_;
  std::vector<std::pair<int, int>> path_;
};

std::ostream& operator<<(std::ostream& os, const Map& map);
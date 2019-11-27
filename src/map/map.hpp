#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "tile.hpp"

class Map {
 public:
  Map();
  void Load(const std::string& filename);
  void Draw(sf::RenderWindow& window);
  int GetWidth() const;
  int GetHeight() const;
  const std::vector<std::vector<Tile>> GetTiles() const;
  const Tile& operator()(int x, int y);
  const std::pair<int, int> GetEnemySpawn() const;
  const std::pair<int, int> GetPlayerBase() const;
  bool RecalculatePath();
  std::vector<std::pair<int, int>> GetPath();

 private:
  std::vector<std::vector<Tile>> tiles_;
  std::pair<int, int> enemy_spawn_;
  std::pair<int, int> player_base_;
  std::vector<std::pair<int, int>> path_;
};

std::ostream& operator<<(std::ostream& os, const Map& map);
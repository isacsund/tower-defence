#pragma once
#include <iostream>

enum PathTypes { Path, Empty, EnemySpawn, PlayerBase };

class Tile {
 public:
  Tile(PathTypes type = Empty);
  const PathTypes GetType() const;
  const std::string& GetTexture() const;

 private:
  PathTypes type_;
  std::string texture_;
};

std::ostream& operator<<(std::ostream& os, const Tile& tile);

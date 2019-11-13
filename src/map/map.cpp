#include "map.hpp"

Map::Map(const std::string& filename) {
  std::ifstream is(filename);
  if (!is.is_open()) {
    std::cout << "Failed to open " << filename << std::endl;
  } else {
    std::string line;
    int i = 0;
    while (std::getline(is, line)) {
      tiles_.push_back(std::vector<Tile>());
      for (auto c : line) {
        switch (c) {
          case '0':
            tiles_[i].push_back(Tile(Empty));
            break;
          case '#':
            tiles_[i].push_back(Tile(Path));
            break;
          case 'B':
            tiles_[i].push_back(Tile(PlayerBase));
            break;
          case 'S':
            tiles_[i].push_back(Tile(EnemySpawn));
            break;
          default:
            tiles_[i].push_back(Tile(Empty));
            break;
        }
      }
      i++;
    }
  }
  is.close();
}

const int Map::GetHeight() const { return tiles_.size(); }

const int Map::GetWidth() const { return tiles_[0].size(); }

const std::vector<std::vector<Tile>> Map::GetTiles() const { return tiles_; };

const Tile& Map::operator()(int x, int y) const { return tiles_[y][x]; };

std::ostream& operator<<(std::ostream& os, const Map& map) {
  for (auto row : map.GetTiles()) {
    for (auto tile : row) {
      os << tile;
    }
    os << std::endl;
  }
  return os;
}

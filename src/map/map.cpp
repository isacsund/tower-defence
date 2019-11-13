#include "map.hpp"
#include "pathfinder.hpp"
Map::Map(const std::string& filename) {
  std::ifstream is(filename);
  if (!is.is_open()) {
    std::cout << "Failed to open " << filename << std::endl;
  } else {
    std::string line;
    int i = 0;
    while (std::getline(is, line)) {
      tiles_.push_back(std::vector<Tile>());
      int j = 0;
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
            player_base_ = std::pair<int, int>(j, i);
            break;
          case 'S':
            tiles_[i].push_back(Tile(EnemySpawn));
            enemy_spawn_ = std::pair<int, int>(j, i);
            break;
          default:
            tiles_[i].push_back(Tile(Empty));
            break;
        }
        j++;
      }
      i++;
    }
  }
  is.close();
  RecalculatePath();
}

const int Map::GetHeight() const { return tiles_.size(); }

const int Map::GetWidth() const { return tiles_[0].size(); }

const std::pair<int, int> Map::GetEnemySpawn() const { return enemy_spawn_; }

const std::pair<int, int> Map::GetPlayerBase() const { return player_base_; }

const std::vector<std::vector<Tile>> Map::GetTiles() const { return tiles_; };

const Tile& Map::operator()(int x, int y) const { return tiles_[y][x]; };

bool Map::RecalculatePath() {
  auto new_path = Pathfinder::GetPath(*this);
  if (new_path.size() <= 1) {
    std::cout << "Error calculating the enemy path" << std::endl;
    return false;
  } else {
    path_ = new_path;
    return true;
  }
}

std::vector<std::pair<int, int>> Map::GetPath() { return path_; }

std::ostream& operator<<(std::ostream& os, const Map& map) {
  for (auto row : map.GetTiles()) {
    for (auto tile : row) {
      os << tile;
    }
    os << std::endl;
  }
  return os;
}

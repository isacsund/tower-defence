#include "map.hpp"
#include "pathfinder.hpp"
Map::Map() {}

void Map::Load(const std::string& filename) {
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

void Map::Draw(sf::RenderWindow& window) {
  auto windowsize = window.getSize();
  int tile_size_x = (windowsize.x - 200) / GetWidth();
  int tile_size_y = (windowsize.y - 200) / GetHeight();
  auto tile_size = std::min(tile_size_x, tile_size_y);

  sf::Sprite* sprite;
  for (int y = 0; y < GetHeight(); y++) {
    for (int x = 0; x < GetWidth(); x++) {
      Tile tile = tiles_[y][x];
      sprite = tile.GetSprite();
      if (sprite->getTexture() != nullptr) {
        sprite->setPosition(x * tile_size, y * tile_size);
        sprite->setScale(
            tile_size / (float)(*sprite->getTexture()).getSize().x,
            tile_size / (float)(*sprite->getTexture()).getSize().y);
        window.draw(*sprite);
      }
    }
  }
}

int Map::GetHeight() const { return tiles_.size(); }

int Map::GetWidth() const { return tiles_[0].size(); }

const std::pair<int, int> Map::GetEnemySpawn() const { return enemy_spawn_; }

const std::pair<int, int> Map::GetPlayerBase() const { return player_base_; }

const std::vector<std::vector<Tile>> Map::GetTiles() const { return tiles_; }

const Tile& Map::operator()(int x, int y) { return tiles_[y][x]; }

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

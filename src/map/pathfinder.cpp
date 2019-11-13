#include "pathfinder.hpp"
#include <math.h>
#include <algorithm>
#include <array>
#include <limits>

namespace Pathfinder {

bool operator<(const Node& l, const Node& r) { return l.f < r.f; }

bool operator==(const Node& l, const Node& r) {
  return (l.x == r.x && l.y == r.y);
}

std::ostream& operator<<(std::ostream& os, const Node& n) {
  os << "(" << n.x << ", " << n.y << ")"
     << "f:" << n.f << ", g:" << n.g << " h:" << n.h;
  if (n.parent == nullptr) {
  } else {
    os << " " << (*(n.parent)).x << " " << (*(n.parent)).y << std::endl;
  }
  return os;
}

// Converts the map to a grid of 0 and 1 based on if its traversable or not by
// enemies
const std::vector<std::vector<bool>> GetGrid(const Map map) {
  std::vector<std::vector<bool>> grid;
  int i = 0;
  for (auto row : map.GetTiles()) {
    grid.push_back(std::vector<bool>());
    for (auto col : row) {
      grid[i].push_back(IsTraversable(col.GetType()));
    }
    i++;
  }
  return grid;
}

bool Exists(std::vector<Node> nodes, Node node) {
  for (auto it = nodes.begin(); it != nodes.end(); it++) {
    if ((*it) == node) return true;
  }
  return false;
}

// Get the path using A* search algorithm
const std::vector<std::pair<int, int>> GetPath(const Map map) {
  auto grid = GetGrid(map);

  auto enemy_base = map.GetEnemySpawn();
  auto player_base = map.GetPlayerBase();

  Node start;
  start.x = enemy_base.first;
  start.y = enemy_base.second;

  Node dest;
  dest.x = player_base.first;
  dest.y = player_base.second;

  std::vector<Node> open_list;
  std::vector<Node> closed_list;

  std::vector<std::pair<int, int>> path;
  std::vector<std::pair<int, int>> neighbour_offsets = {
      {0, -1}, {0, 1}, {-1, 0}, {1, 0}};

  open_list.push_back(start);
  bool found = false;

  while (!open_list.empty()) {
    if (!found) {
      auto cur_node_it = std::min_element(open_list.begin(), open_list.end());
      Node cur_node = *cur_node_it;
      open_list.erase(cur_node_it);

      if (!Exists(closed_list, cur_node)) {
        closed_list.push_back(cur_node);
        // Found a path to the destination tile
        if ((cur_node) == dest) {
          Node cur = cur_node;
          // Recreate the path by traversing the parents
          while (cur.parent != nullptr) {
            path.push_back({cur.x, cur.y});
            cur = *cur.parent;
          }
          found = true;
        }
        std::vector<Node> children;
        // Add each neighbour to tiles to check
        for (auto offset : neighbour_offsets) {
          int x = cur_node.x + offset.first;
          int y = cur_node.y + offset.second;
          // Make sure neighbour is inside map
          if (x < grid[0].size() && x >= 0 && y < grid.size() && y >= 0) {
            // Check if neighbour tile is traversable
            if (grid[y][x]) {
              Node child;
              child.x = x;
              child.y = y;
              child.parent = std::make_shared<Node>(cur_node);
              children.push_back(child);
            }
          }
        }
        for (auto c = children.begin(); c != children.end(); c++) {
          if (!Exists(closed_list, *c)) {
            c->g = cur_node.g + 1;
            c->h = sqrt(pow(c->x - dest.x, 2) + pow(c->y - dest.y, 2));
            c->f = c->g + c->h;
          }
          bool f = false;
          for (auto o : open_list) {
            if (o == *c && c->g > o.g) {
              f = true;
            }
          }
          if (!f) {
            open_list.push_back(*c);
          }
        }
      }
    } else {
      open_list.clear();
    }
  }
  path.push_back({start.x, start.y});
  std::reverse(path.begin(), path.end());
  return path;
}

}  // namespace Pathfinder

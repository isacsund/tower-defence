#pragma once
#include <memory>
#include <vector>
#include "map.hpp"

namespace Pathfinder {
struct Node {
  int x, y;
  float g = 0.0;
  float h = 0.0;
  float f = 0.0;
  std::shared_ptr<Node> parent = nullptr;
};

bool operator<(const Node& l, const Node& r);
bool operator==(const Node& l, const Node& r);

const std::vector<std::vector<bool>> GetGrid(const Map map);

bool Exists(std::vector<Node> nodes, Node node);

const std::vector<std::pair<int, int>> GetPath(Map map);
};  // namespace Pathfinder

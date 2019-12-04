#pragma once
#include <SFML/Graphics.hpp>
#include <boost/optional.hpp>
#include <vector>
#include "../enemy/enemy.hpp"
#include "../gui/button.hpp"
#include "../map/map.hpp"
#include "../tower/tower.hpp"
#include "game_state.hpp"

class PlayState : public GameState {
 public:
  PlayState(Game* game, Map map);
  virtual void Draw();
  virtual void HandleInput();
  void Tick();
  void AddToSpawnQueue(std::vector<Enemy> enemies);
  void FindEnemies();
  void HandleMapClick(int x, int y);
  void HandleGuiClick(sf::Vector2f mouse_position);
  void InitGUI();
  int GetTileSize() const;

 private:
  Map map_;
  std::vector<Enemy> enemies_;
  std::deque<Enemy> spawn_queue_;
  std::map<std::pair<int, int>, Tower> towers_;
  sf::View view_;
  sf::Sprite background_;
  sf::Font font_;
  std::map<std::string, Button> buttons_;
  boost::optional<Tower> active_tower_;
  sf::Clock clock_;
  float last_spawn_;
  int wave_;
};
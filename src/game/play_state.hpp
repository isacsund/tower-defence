#pragma once
#include <SFML/Graphics.hpp>
#include <boost/optional.hpp>
#include <vector>
#include "../gui/button.hpp"
#include "../map/map.hpp"
#include "game_state.hpp"

class PlayState : public GameState {
 public:
  PlayState(Game* game, Map map);
  virtual void Draw();
  virtual void HandleInput();
  void Tick();
  void SpawnEnemies(std::vector<Enemy> enemies);
  void DrawTowerAtMousePos();
  int GetTileSize() const;
  void InitGUI();

 private:
  Map map_;
  std::vector<Enemy> enemies_;
  std::vector<Tower> towers_;
  sf::View view_;
  sf::Sprite background_;
  sf::Font font_;
  std::map<std::string, Button> buttons_;
  boost::optional<Tower> active_tower_;
  sf::Clock clock_;
};
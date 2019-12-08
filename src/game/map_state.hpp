#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include "../gui/gui.hpp"
#include "../map/map.hpp"
#include "game_state.hpp"

class MapState : public GameState {
 public:
  MapState(Game* game);
  virtual void Draw();
  virtual void HandleInput();
  void InitGUI();
  void LoadGame();

 private:
  sf::View view_;
  sf::Sprite background_;
  sf::Font font_;
  Gui gui_;
  Map map_;
  int map_count;
};
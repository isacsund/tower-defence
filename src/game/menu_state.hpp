#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include "../gui/gui.hpp"
#include "game_state.hpp"

class MenuState : public GameState {
 public:
  MenuState(Game* game);
  virtual void Draw();
  virtual void HandleInput();
  void LoadGame();

 private:
  sf::View view_;
  sf::Sprite background_;
  sf::Font font_;
  Gui menu_;
};
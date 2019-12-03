#pragma once
#include "game.hpp"

class Game;

class GameState {
 public:
  Game* game;
  virtual void Draw() = 0;
  virtual void HandleInput() = 0;
};

#pragma once
#include <SFML/Graphics.hpp>
#include <stack>
#include "game_state.hpp"

class GameState;

class Game {
 public:
  Game();
  ~Game();
  void PushState(GameState* state);
  void PopState();
  void ChangeState(GameState* state);
  GameState* PeekState();
  void Run();

  sf::RenderWindow window;

 private:
  std::stack<GameState*> states_;
};
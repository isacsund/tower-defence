#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <stack>
#include "../enemy/enemy.hpp"
#include "../map/map.hpp"
#include "../tower/tower.hpp"
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
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
  void DrawAll();
  void DrawMap();
  void DrawEnemies();
  void DrawTowers();
  void Tick();
  void FindEnemies();

  sf::RenderWindow window;

 private:
  std::stack<GameState*> states_;
  Map map_;
  sf::View view_;
  std::map<const std::string, sf::Texture> textures_;
  std::vector<Enemy> enemies_;
  std::vector<Tower> towers_;
  sf::Clock clock_;
};
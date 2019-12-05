#pragma once
#include <SFML/Audio.hpp>
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
  float GetMusicVolume() const;
  void SetMusicVolume(float volume);
  sf::RenderWindow window;
  sf::Music music;

 private:
  std::stack<GameState*> states_;
};
#include "game.hpp"
#include "../configuration/configmanager.hpp"

Game::Game() {
  window.create(sf::VideoMode(800, 600), "Tower Defence");
  window.setFramerateLimit(60);

  std::string config_error;
  if (!config_manager->ParseFile("settings.json", config_error)) {
    std::cout << "Failed to parse configuration file." << std::endl;
  }

  if (!music.openFromFile("audio/medieval_music.ogg"))
    std::cout << "Could not load music" << std::endl;
  music.setLoop(true);
  music.setVolume(5.f);
}

Game::~Game() {
  while (!states_.empty()) PopState();
}

void Game::PushState(GameState* state) { states_.push(state); }

void Game::PopState() { states_.pop(); }

void Game::ChangeState(GameState* state) {
  if (!states_.empty()) PopState();
  PushState(state);
}

GameState* Game::PeekState() {
  if (states_.empty()) return nullptr;
  return states_.top();
}

void Game::Run() {
  music.play();
  while (window.isOpen()) {
    if (PeekState() == nullptr) continue;
    PeekState()->HandleInput();
    window.clear();
    PeekState()->Draw();
    window.display();
  }
}

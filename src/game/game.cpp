#include "game.hpp"
#include <math.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "../configuration/configmanager.hpp"
#include "texturemanager.hpp"

Game::Game() : map_(), view_() {
  window.create(sf::VideoMode(800, 600), "Tower Defence");
  window.setFramerateLimit(60);

  std::string config_error;
  if (!config_manager->ParseFile("settings.json", config_error)) {
    std::cout << "Failed to parse configuration file." << std::endl;
  }
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
  while (window.isOpen()) {
    if (PeekState() == nullptr) continue;
    PeekState()->HandleInput();
    window.clear();
    PeekState()->Draw();
    window.display();
  }
}
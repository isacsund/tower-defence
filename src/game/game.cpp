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
    // DrawAll();
    // Tick();
    window.display();
  }
}

void Game::Tick() {
  auto path = map_.GetPath();
  auto player_base = map_.GetPlayerBase();
  for (auto it = enemies_.begin(); it != enemies_.end(); it++) {
    if (it->IsAlive()) {
      it->Move(path);
      if (it->GetTile() == player_base) {
        it->SetHp(0);
      }
    }
  }
  FindEnemies();
}

/*

void Game::DrawTowers() {
  int tower_size = GetTileSize();
  sf::Sprite* sprite;
  for (auto& tower : towers_) {
    sprite = tower.GetSprite();
    if (sprite->getTexture() != nullptr) {
      sprite->setPosition(tower.GetPosition().first * tower_size,
                          tower.GetPosition().second * tower_size);
      sprite->setScale(tower_size / (float)(*sprite->getTexture()).getSize().x,
                       tower_size / (float)(*sprite->getTexture()).getSize().y);
      window.draw(*sprite);
    }
  }
}*/

void Game::FindEnemies() {
  auto cur_time = clock_.getElapsedTime().asSeconds();
  float closest_distance = std::numeric_limits<float>::max();
  Enemy* closest_enemy = nullptr;
  for (auto& tower : towers_) {
    float range = tower.GetRange();
    auto tower_pos = tower.GetPosition();
    for (auto& enemy : enemies_) {
      auto enemy_pos = enemy.GetPosition();
      float distance = sqrt(pow(tower_pos.first + 0.5 - enemy_pos.first, 2) +
                            pow(tower_pos.second + 0.5 - enemy_pos.second, 2));
      if (distance <= range && distance < closest_distance && enemy.IsAlive()) {
        closest_enemy = &enemy;
        closest_distance = distance;
      }
    }
    if ((closest_enemy) &&
        (cur_time - tower.GetLastAttack() > tower.GetAttSpeed())) {
      tower.SetLastAttack(cur_time);
      tower.Attack(*closest_enemy);
    }
  }
}
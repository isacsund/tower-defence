#include "game.hpp"
#include <math.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "../configuration/configmanager.hpp"
#include "texturemanager.hpp"

Game::Game() : map_(), view_() {
  window.create(sf::VideoMode(800, 600), "Tower Defence");
  window.setFramerateLimit(60);

  auto spawn = map_.GetEnemySpawn();
  enemies_.push_back(Enemy(200, 1, spawn.first + 0.5, spawn.second + 0.5));
  towers_.push_back(Tower(10, 10, 1, 1, 2));
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
void Game::DrawAll() {
  DrawEnemies();
  DrawTowers();
}

void Game::DrawEnemies() {
  int enemy_size = GetTileSize();
  const int hp_bar_width = enemy_size / 2;
  const int hp_bar_height = enemy_size / 10;
  sf::Sprite* sprite;
  for (auto& enemy : enemies_) {
    if (enemy.IsAlive()) {
      // Draw hp bar
      float hp_ratio = enemy.GetHp() / enemy.GetMaxHp();
      sf::RectangleShape hp_bar_green;
      sf::RectangleShape hp_bar_red;
      hp_bar_green.setFillColor(sf::Color::Green);
      hp_bar_red.setFillColor(sf::Color::Red);
      hp_bar_red.setSize(sf::Vector2f(hp_bar_width, hp_bar_height));
      hp_bar_green.setSize(
          sf::Vector2f(hp_bar_width * hp_ratio, hp_bar_height));
      hp_bar_green.setPosition(
          enemy.GetPosition().first * enemy_size - hp_bar_width / 2,
          enemy.GetPosition().second * enemy_size - enemy_size / 2);
      hp_bar_red.setPosition(hp_bar_green.getPosition());
      window.draw(hp_bar_red);
      window.draw(hp_bar_green);

      // Draw enemy
      sprite = enemy.GetSprite();
      sprite->setPosition(
          enemy.GetPosition().first * enemy_size - enemy_size / 2,
          enemy.GetPosition().second * enemy_size - enemy_size / 2);
      sprite->setScale(enemy_size / (float)(*sprite->getTexture()).getSize().x,
                       enemy_size / (float)(*sprite->getTexture()).getSize().y);
      window.draw(*sprite);
    }
  }
}

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

int Game::GetTileSize() const {
  auto windowsize = window.getSize();
  int tile_size_x = (windowsize.x - 200) / map_.GetWidth();
  int tile_size_y = (windowsize.y - 200) / map_.GetHeight();
  return std::min(tile_size_x, tile_size_y);
}

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
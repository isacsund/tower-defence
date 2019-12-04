#include "play_state.hpp"
#include <math.h>
#include <SFML/Graphics.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include "../configuration/configmanager.hpp"
#include "../enemy/enemy.hpp"
#include "game_state.hpp"
#include "menu_state.hpp"

PlayState::PlayState(Game* game, Map map) {
  this->game = game;
  map_ = map;

  sf::Vector2f window_size = sf::Vector2f(this->game->window.getSize());
  sf::View view_(sf::FloatRect(0, 0, window_size.x, window_size.y));
  this->game->window.setView(view_);
  if (!font_.loadFromFile("sprites/Arial.ttf")) {
    std::cout << "Failed to load font";
  }
  InitGUI();
}

void PlayState::Draw() {
  map_.Draw(this->game->window);
  for (auto& button : buttons_) {
    this->game->window.draw(button.second);
  }
  for (auto& enemy : boost::adaptors::reverse(enemies_)) {
    if (enemy.IsAlive()) {
      enemy.SetPosition(
          enemy.GetPosition().first * GetTileSize() - GetTileSize() / 2,
          enemy.GetPosition().second * GetTileSize() - GetTileSize() / 2);
      enemy.SetScale(GetTileSize() / (float)(enemy.GetTexture()).getSize().x,
                     GetTileSize() / (float)(enemy.GetTexture()).getSize().y);
      this->game->window.draw(enemy);
    }
  }
  for (auto& tower : towers_) {
    tower.second.SetPosition(tower.second.GetPosition().first * GetTileSize(),
                             tower.second.GetPosition().second * GetTileSize());
    tower.second.SetScale(
        GetTileSize() / (float)(tower.second.GetTexture()).getSize().x,
        GetTileSize() / (float)(tower.second.GetTexture()).getSize().y);
    this->game->window.draw(tower.second);
  }
  if (active_tower_.has_value()) {
    active_tower_->SetPosition(
        sf::Mouse::getPosition(this->game->window).x - GetTileSize() / 2,
        sf::Mouse::getPosition(this->game->window).y - GetTileSize() / 2);
    this->game->window.draw(active_tower_.get());
  }

  Tick();
}

void PlayState::HandleInput() {
  sf::Event event;

  while (this->game->window.pollEvent(event)) {
    switch (event.type) {
      /* Close the window */
      case sf::Event::Closed: {
        game->window.close();
        break;
      }
      /* Resize the window */
      case sf::Event::Resized: {
        view_.reset(sf::FloatRect(0, 0, event.size.width, event.size.height));
        this->game->window.setView(view_);
        buttons_.at("Tower1").SetPosition(
            sf::Vector2f(GetTileSize() * map_.GetWidth(), 0));
        buttons_.at("Wave").SetPosition(
            sf::Vector2f(GetTileSize() * map_.GetWidth(), 150));
        break;
      }
      case sf::Event::MouseButtonPressed: {
        sf::Vector2f mouse_position =
            sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
        if (event.mouseButton.button == sf::Mouse::Left) {
          int tile_size = GetTileSize();
          int tile_x = mouse_position.x / tile_size;
          int tile_y = mouse_position.y / tile_size;
          if (tile_x >= 0 && tile_y >= 0 && tile_x < map_.GetWidth() &&
              tile_y < map_.GetHeight()) {
            HandleMapClick(tile_x, tile_y);
          } else {
            HandleGuiClick(mouse_position);
          }
        }
        break;
      }
      case sf::Event::KeyPressed: {
        if (event.key.code == sf::Keyboard::Escape) {
          std::vector<Enemy> enemies = map_.LoadWave(1);
          for (auto enemy : enemies) {
            std::cout << enemy << std::endl;
          }
        }
        break;
      }
      default:
        break;
    }
  }
}

void PlayState::Tick() {
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

void PlayState::SpawnEnemies(std::vector<Enemy> enemies) {
  // auto cur_time = clock_.getElapsedTime().asSeconds();
  for (size_t i = 0; i < 1; i++) {
    enemies_.push_back(enemies[i]);
  }
}

void PlayState::FindEnemies() {
  auto cur_time = clock_.getElapsedTime().asSeconds();
  float closest_distance = std::numeric_limits<float>::max();
  Enemy* closest_enemy = nullptr;
  for (auto& tower : towers_) {
    float range = tower.second.GetRange();
    auto tower_pos = tower.second.GetPosition();
    for (auto& enemy : enemies_) {
      auto enemy_pos = enemy.GetPosition();
      float distance = sqrt(pow(tower_pos.first + 0.5 - enemy_pos.first, 2) +
                            pow(tower_pos.second + 0.5 - enemy_pos.second, 2));
      if (distance <= range && distance < closest_distance && enemy.IsAlive()) {
        closest_enemy = &enemy;
        closest_distance = distance;
      }
    }
    if ((closest_enemy) && (cur_time - tower.second.GetLastAttack() >
                            tower.second.GetAttSpeed())) {
      tower.second.SetLastAttack(cur_time);
      tower.second.Attack(*closest_enemy);
    }
  }
}

void PlayState::HandleMapClick(int x, int y) {
  if (active_tower_.has_value() && map_(x, y).GetType() == Empty) {
    auto tower = Tower(300, 10, 1, x, y, GetTileSize());
    towers_.insert({{x, y}, tower});
    tower.SetActive();
    active_tower_ = boost::none;
  } else if (towers_.count({x, y})) {
    for (auto& tower : towers_) {
      tower.second.SetInactive();
    }
    towers_.at({x, y}).SetActive();
  } else {
    for (auto& tower : towers_) {
      tower.second.SetInactive();
    }
  }
}
void PlayState::HandleGuiClick(sf::Vector2f mouse_position) {
  if (buttons_.at("Tower1").Contains(mouse_position)) {
    for (auto& tower : towers_) {
      tower.second.SetInactive();
    }
    active_tower_ =
        Tower(300, 10, 1, mouse_position.x, mouse_position.y, GetTileSize());
    active_tower_->SetActive();
    std::cout << "Pressed Tower1 button" << std::endl;
  } else if (buttons_.at("Wave").Contains(mouse_position)) {
    SpawnEnemies(map_.LoadWave(1));
  }
}

void PlayState::InitGUI() {
  buttons_.emplace("Tower1",
                   Button("Tower1", font_,
                          sf::Vector2f(map_.tile_size * (map_.GetWidth()), 0),
                          "sprites/basic_tower.png"));
  buttons_.emplace("Wave",
                   Button("Next wave", font_,
                          sf::Vector2f(map_.tile_size * (map_.GetWidth()), 150),
                          "sprites/button.png"));
}

int PlayState::GetTileSize() const {
  auto windowsize = this->game->window.getSize();
  int tile_size_x = (windowsize.x - 200) / map_.GetWidth();
  int tile_size_y = (windowsize.y - 200) / map_.GetHeight();
  return std::min(tile_size_x, tile_size_y);
}

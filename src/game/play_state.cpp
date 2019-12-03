#include "play_state.hpp"
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
          enemy.GetPosition().first * map_.tile_size - map_.tile_size / 2,
          enemy.GetPosition().second * map_.tile_size - map_.tile_size / 2,
          map_.tile_size);
      this->game->window.draw(enemy);
    }
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
            sf::Vector2f((map_.tile_size) * map_.GetHeight(), 0));
        buttons_.at("Wave").SetPosition(
            sf::Vector2f((map_.tile_size) * map_.GetHeight(), 150));
        break;
      }
      case sf::Event::MouseButtonPressed: {
        sf::Vector2f mouse_position =
            sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
        if (event.mouseButton.button == sf::Mouse::Left) {
          int tile_size = map_.tile_size;
          int tile_x = mouse_position.x / tile_size;
          int tile_y = mouse_position.y / tile_size;
          if (tile_x >= 0 && tile_y >= 0 && tile_x < map_.GetWidth() &&
              tile_y < map_.GetHeight()) {
            // TODO: Add tower placement
          } else if (buttons_.at("Tower1").Contains(mouse_position)) {
            std::cout << "Pressed Tower1 button" << std::endl;
          } else if (buttons_.at("Wave").Contains(mouse_position)) {
            SpawnEnemies(map_.LoadWave(1));
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
  // FindEnemies();
}

void PlayState::SpawnEnemies(std::vector<Enemy> enemies) {
  // auto cur_time = clock_.getElapsedTime().asSeconds();
  for (size_t i = 0; i < 20; i++) {
    enemies_.push_back(enemies[i]);
  }
}

void PlayState::InitGUI() {
  buttons_.emplace("Tower1",
                   Button("Tower1", font_,
                          sf::Vector2f(map_.tile_size * (map_.GetWidth()), 0),
                          "sprites/basic_tower.png"));
  buttons_.emplace("Wave",
                   Button("Next vave", font_,
                          sf::Vector2f(map_.tile_size * (map_.GetWidth()), 150),
                          "sprites/button.png"));
}

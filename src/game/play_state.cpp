#include "play_state.hpp"
#include <SFML/Graphics.hpp>
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
  // view_.reset(sf::FloatRect(0, 0, window_size.x, window_size.y));
  game->window.setView(view_);
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
            sf::Vector2f((GetTileSize() - 1) * map_.GetHeight(), 50));
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
            // TODO: Add tower placement
          } else if (buttons_.at("Tower1").Contains(mouse_position)) {
            std::cout << "Pressed Tower1 button" << std::endl;
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

void PlayState::InitGUI() {
  buttons_.emplace(
      "Tower1",
      Button("Tower1", font_,
             sf::Vector2f((GetTileSize() - 1) * map_.GetHeight(), 50)));
}

int PlayState::GetTileSize() const {
  auto windowsize = this->game->window.getSize();
  int tile_size_x = (windowsize.x - 200) / map_.GetWidth();
  int tile_size_y = (windowsize.y) / map_.GetHeight();
  return std::min(tile_size_x, tile_size_y);
}

#include "play_state.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include "../configuration/configmanager.hpp"
#include "../enemy/enemy.hpp"
#include "game_state.hpp"
#include "menu_state.hpp"

PlayState::PlayState(Game* game) {
  this->game = game;

  sf::Vector2f window_size = sf::Vector2f(this->game->window.getSize());
  sf::View view_(sf::FloatRect(0, 0, window_size.x, window_size.y));
  // view_.reset(sf::FloatRect(0, 0, window_size.x, window_size.y));
  game->window.setView(view_);
  this->game->window.setView(view_);
}

void PlayState::Draw() { map.Draw(this->game->window); }

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
        break;
      }
      case sf::Event::MouseButtonPressed: {
        sf::Vector2f mouse_position =
            sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
        if (event.mouseButton.button == sf::Mouse::Left) {
          int tile_size = GetTileSize();
          int tile_x = mouse_position.x / tile_size;
          int tile_y = mouse_position.y / tile_size;
          if (tile_x >= 0 && tile_y >= 0 && tile_x < map.GetWidth() &&
              tile_y < map.GetHeight()) {
            // TODO: Add tower placement
          }
        }
        break;
      }
      case sf::Event::KeyPressed: {
        if (event.key.code == sf::Keyboard::Escape) {
          std::vector<Enemy> enemies = map.LoadWave(1);
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

int PlayState::GetTileSize() const {
  auto windowsize = this->game->window.getSize();
  int tile_size_x = (windowsize.x - 200) / map.GetWidth();
  int tile_size_y = (windowsize.y) / map.GetHeight();
  return std::min(tile_size_x, tile_size_y);
}
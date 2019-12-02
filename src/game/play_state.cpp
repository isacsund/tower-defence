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

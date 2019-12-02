#include "play_state.hpp"
#include <SFML/Graphics.hpp>
#include "../configuration/configmanager.hpp"
#include "game_state.hpp"
#include "menu_state.hpp"
#include "texturemanager.hpp"

PlayState::PlayState(Game* game) {
  this->game = game;

  std::string config_error;
  if (!config_manager->ParseFile("settings.json", config_error)) {
    std::cout << "Failed to parse configuration file." << std::endl;
  }

  sf::Vector2f window_size = sf::Vector2f(this->game->window.getSize());
  sf::View view_(sf::FloatRect(0, 0, window_size.x, window_size.y));
  // view_.reset(sf::FloatRect(0, 0, window_size.x, window_size.y));
  game->window.setView(view_);
  this->game->window.setView(view_);
  map_.Load(config_manager->GetValueOrDefault<std::string>("maps/01/file",
                                                           "maps/01/file"));
}

void PlayState::Draw() { map_.Draw(this->game->window); }

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
        if (event.key.code == sf::Keyboard::Escape)
          this->game->PushState(new MenuState(this->game));
        break;
      }
      default:
        break;
    }
  }
}

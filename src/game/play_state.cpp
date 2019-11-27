#include "play_state.hpp"
#include <SFML/Graphics.hpp>
#include "game_state.hpp"
#include "texturemanager.hpp"

PlayState::PlayState(Game* game) {
  this->game = game;
  background_.setTexture(texture_manager.GetTexture("sprites/basic_enemy.png"));
}

void PlayState::Draw() {
  this->game->window.setView(view_);
  this->game->window.draw(background_);
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
        view_.setSize(event.size.width, event.size.height);
        background_.setPosition(
            this->game->window.mapPixelToCoords(sf::Vector2i(0, 0), view_));
        background_.setScale(float(event.size.width) /
                                 float(background_.getTexture()->getSize().x),
                             float(event.size.height) /
                                 float(background_.getTexture()->getSize().y));
        break;
      }
      case sf::Event::KeyPressed: {
        if (event.key.code == sf::Keyboard::Escape) this->game->window.close();
        break;
      }
      default:
        break;
    }
  }
}

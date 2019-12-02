#include "menu_state.hpp"
#include <iostream>
#include "play_state.hpp"
#include "texturemanager.hpp"

MenuState::MenuState(Game* game) {
  this->game = game;
  background_.setTexture(texture_manager.GetTexture("sprites/background.png"));
  background_.setScale(float(this->game->window.getSize().x) /
                           float(background_.getTexture()->getSize().x),
                       float(this->game->window.getSize().y) /
                           float(background_.getTexture()->getSize().y));

  if (!font_.loadFromFile("sprites/Arial.ttf")) {
    std::cout << "Failed to load font";
  }

  sf::Vector2f window_size = sf::Vector2f(this->game->window.getSize());

  buttons_.push_back(
      Button("Test", font_, sf::Vector2f(window_size.x / 2, 200)));
}

void MenuState::Draw() {
  this->game->window.setView(view_);
  this->game->window.draw(background_);
  for (auto& button : buttons_) {
    this->game->window.draw(button);
  }
}

void MenuState::HandleInput() {
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
        if (event.key.code == sf::Keyboard::Escape)
          this->game->PushState(new PlayState(this->game));
        break;
      }
      default:
        break;
    }
  }
}

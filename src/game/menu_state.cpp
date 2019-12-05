#include "menu_state.hpp"
#include <iostream>
#include "../configuration/configmanager.hpp"
#include "../map/map.hpp"
#include "play_state.hpp"
#include "texturemanager.hpp"
#include "wavemanager.hpp"

MenuState::MenuState(Game* game) {
  this->game = game;
  sf::Vector2u window_size = this->game->window.getSize();
  sf::View view_(sf::FloatRect(0, 0, window_size.x, window_size.y));
  game->window.setView(view_);
  background_.setTexture(texture_manager.GetTexture("sprites/background.png"));
  background_.setScale(float(this->game->window.getSize().x) /
                           float(background_.getTexture()->getSize().x),
                       float(this->game->window.getSize().y) /
                           float(background_.getTexture()->getSize().y));

  if (!font_.loadFromFile("sprites/Arial.ttf")) {
    std::cout << "Failed to load font";
  }

  menu_.Add(
      "Play",
      GuiEntry(sf::Vector2f((window_size.x / 2) - 84.5, window_size.y / 2),
               std::string("Play"),
               texture_manager.GetTexture("sprites/button.png"), font_));
}

void MenuState::Draw() {
  this->game->window.draw(background_);
  this->game->window.draw(menu_);
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
        view_.reset(sf::FloatRect(0, 0, event.size.width, event.size.height));
        game->window.setView(view_);

        background_.setPosition(
            this->game->window.mapPixelToCoords(sf::Vector2i(0, 0), view_));
        background_.setScale(float(event.size.width) /
                                 float(background_.getTexture()->getSize().x),
                             float(event.size.height) /
                                 float(background_.getTexture()->getSize().y));
        menu_.Get("Play").SetPosition(
            sf::Vector2f((event.size.width / 2) - 84.5, event.size.height / 2));

        break;
      }
      case sf::Event::MouseButtonPressed: {
        sf::Vector2f mouse_position =
            sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
        if (event.mouseButton.button == sf::Mouse::Left) {
          if (menu_.Get("Play").Contains(mouse_position)) {
            LoadGame();
          }
        }
        break;
      }
      case sf::Event::KeyPressed: {
        if (event.key.code == sf::Keyboard::Escape) {
        }
        break;
      }
      default:
        break;
    }
  }
}

void MenuState::LoadGame() {
  Map map = Map();
  map.SetName("01");
  map.Load(config_manager->GetValueOrDefault<std::string>(
      "maps/" + map.GetName() + "/file", "maps/01/file"));
  map.tile_size =
      std::min((this->game->window.getSize().x - 200) / map.GetWidth(),
               (this->game->window.getSize().y) / map.GetHeight());
  wave_manager.ParseFile(
      "maps/" + map.GetName() + "/" +
      config_manager->GetValueOrDefault<std::string>(
          "maps/" + map.GetName() + "/waves", "maps/01/waves"));
  this->game->PushState(new PlayState(this->game, map));
}

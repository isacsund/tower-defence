#include "map_state.hpp"
#include <iostream>
#include "../configuration/configmanager.hpp"
#include "../map/map.hpp"
#include "play_state.hpp"
#include "texturemanager.hpp"
#include "wavemanager.hpp"

MapState::MapState(Game* game) : map_() {
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

  InitGUI();
}

void MapState::Draw() {
  this->game->window.draw(background_);
  this->game->window.draw(gui_);
  // Enable the play button only if a map has been picked
  if (map_.GetName().empty()) gui_.Get("play").Disable();
}

void MapState::HandleInput() {
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

        gui_.Get("select_map")
            .SetPosition(sf::Vector2f((this->game->window.getSize().x / 2 -
                                       (gui_.Get("select_map").GetWidth() / 2)),
                                      this->game->window.getSize().y / 4));

        gui_.Get("play").SetPosition(
            sf::Vector2f((this->game->window.getSize().x / 2) - 84.5,
                         this->game->window.getSize().y * 3 / 4));

        int margin = 10;
        int row = 0;
        int count = 0;
        for (int i = 1; i <= map_count; i++) {
          auto map_name = std::to_string(i);
          // Add leading zero to i
          if (i < 10) {
            map_name = "0" + map_name;
          }
          // If we have 5 maps on one row, switch to the next
          if (count % 5 == 0) {
            row++;
          }
          std::cout << map_name << std::endl;

          gui_.Get(map_name).SetPosition(
              sf::Vector2f((this->game->window.getSize().x / 4) - 84.5 +
                               (count % 5 * 169) + (count % 5 * margin),
                           (this->game->window.getSize().y / 3) + (100 * row)));

          count++;
        }

        break;
      }
      case sf::Event::MouseButtonPressed: {
        sf::Vector2f mouse_position =
            sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
        if (event.mouseButton.button == sf::Mouse::Left) {
          if (gui_.Get("play").Contains(mouse_position)) {
            LoadGame();
          }
          // Check if the click is on a map
          for (int i = 1; i <= map_count; i++) {
            auto map_name = std::to_string(i);
            // Add leading zero to i
            if (i < 10) {
              map_name = "0" + map_name;
            }
            if (gui_.Get(map_name).Contains(mouse_position)) {
              // Unhighlight all buttons
              for (int j = 1; j <= map_count; j++) {
                auto name = std::to_string(j);
                // Add leading zero to j
                if (j < 10) {
                  name = "0" + name;
                }
                gui_.Get(name).Unhighlight();
              }

              map_.SetName(map_name);
              gui_.Get(map_name).Highlight();
              gui_.Get("play").Enable();
            }
          }
        }
        break;
      }
      case sf::Event::KeyPressed: {
        if (event.key.code == sf::Keyboard::Escape) {
          float vol = game->music.getVolume();
          std::cout << "Volume: " << vol << std::endl;
          switch (event.key.code) {
            case sf::Keyboard::M:
              if (game->music.getStatus() != sf::Music::Status::Paused) {
                game->music.pause();
              } else {
                game->music.play();
              }
              break;
            case sf::Keyboard::Add:
              if (vol + 5 <= 100) vol += 5;
              game->music.setVolume(vol);
              break;
            case sf::Keyboard::Subtract:
              if (vol - 5 >= 0) vol -= 5;
              game->music.setVolume(vol);
              break;
            default:
              break;
          }
          break;
        }
        break;
      }
      default:
        break;
    }
  }
}

void MapState::InitGUI() {
  const int margin = 10;

  int row = 0;
  int count = 0;
  for (boost::property_tree::ptree::value_type& map :
       config_manager->GetInstance()->GetSubTree("maps")) {
    // If we have 5 maps on one row, switch to the next
    if (count % 5 == 0) {
      row++;
    }
    gui_.Add(map.first,
             GuiEntry(sf::Vector2f(
                          (this->game->window.getSize().x / 4) - 84.5 +
                              (count % 5 * 169) + (count % 5 * margin),
                          (this->game->window.getSize().y / 3) + (100 * row)),
                      map.first,
                      texture_manager.GetTexture("sprites/button.png"), font_));
    count++;
  }
  map_count = count;

  auto select_map = GuiEntry(sf::Vector2f((this->game->window.getSize().x / 2),
                                          this->game->window.getSize().y / 4),
                             std::string("Select a map"), boost::none, font_);
  select_map.SetPosition(sf::Vector2f(
      (this->game->window.getSize().x / 2 - (select_map.GetWidth() / 2)),
      this->game->window.getSize().y / 4));
  gui_.Add("select_map", select_map);

  gui_.Add("play",
           GuiEntry(sf::Vector2f((this->game->window.getSize().x / 2) - 84.5,
                                 this->game->window.getSize().y * 3 / 4),
                    std::string("Play"),
                    texture_manager.GetTexture("sprites/button.png"), font_));
}

void MapState::LoadGame() {
  map_.Load(config_manager->GetValueOrDefault<std::string>(
      "maps/" + map_.GetName() + "/file", "maps/01/file"));
  map_.tile_size =
      std::min((this->game->window.getSize().x - 200) / map_.GetWidth(),
               (this->game->window.getSize().y - 200) / map_.GetHeight());
  wave_manager.ParseFile(
      "maps/" + map_.GetName() + "/" +
      config_manager->GetValueOrDefault<std::string>(
          "maps/" + map_.GetName() + "/waves", "maps/01/waves"));
  this->game->PushState(new PlayState(this->game, map_));
}

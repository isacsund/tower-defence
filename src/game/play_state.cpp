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
#include "texturemanager.hpp"

PlayState::PlayState(Game* game, Map map)
    : last_spawn_(0), wave_(1), player_(Player("Pelle")) {
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
  for (auto& gui : gui_) {
    this->game->window.draw(gui.second);
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
  if (active_tower_.get_ptr() != 0) {
    active_tower_->SetPosition(
        sf::Mouse::getPosition(this->game->window).x - GetTileSize() / 2,
        sf::Mouse::getPosition(this->game->window).y - GetTileSize() / 2);
    active_tower_->SetScale(
        GetTileSize() / (float)(active_tower_->GetTexture()).getSize().x,
        GetTileSize() / (float)(active_tower_->GetTexture()).getSize().y);
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
        gui_.at("sidegui").Get("tower1").SetPosition(
            sf::Vector2f(GetTileSize() * map_.GetWidth(), 0));
        gui_.at("sidegui").Get("wave").SetPosition(
            sf::Vector2f(GetTileSize() * map_.GetWidth(), 150));
        gui_.at("sidegui").Get("player").SetPosition(
            sf::Vector2f(GetTileSize() * map_.GetWidth(), 250));
        gui_.at("sidegui")
            .Get("nextwave")
            .SetPosition(sf::Vector2f(GetTileSize() * map_.GetWidth(), 350));

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
        float vol = game->music.getVolume();
        std::cout << vol << std::endl;
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
        if (player_.GetLives() > 0) {
          player_.RemoveLives(1);
          gui_.at("sidegui").Get("player").SetTitle(
              "Player: " + player_.GetName() +
              "\nMoney: " + std::to_string(player_.GetMoney()) +
              "\nLives: " + std::to_string(player_.GetLives()));
        }
      }
    }
  }
  FindEnemies();
  auto cur_time = clock_.getElapsedTime().asSeconds();
  if (cur_time - last_spawn_ > 1 && spawn_queue_.size() > 0) {
    enemies_.push_back(spawn_queue_.front());
    spawn_queue_.pop_front();
    last_spawn_ = cur_time;
  }
}

void PlayState::AddToSpawnQueue(std::vector<Enemy> enemies) {
  for (auto& enemy : enemies) {
    spawn_queue_.push_back(enemy);
  }
}

void PlayState::FindEnemies() {
  auto cur_time = clock_.getElapsedTime().asSeconds();
  float closest_distance = std::numeric_limits<float>::max();
  auto player_base = map_.GetPlayerBase();
  Enemy* closest_enemy = nullptr;
  for (auto& tower : towers_) {
    closest_enemy = nullptr;
    closest_distance = std::numeric_limits<float>::max();
    float range = tower.second.GetRange();
    auto tower_pos = tower.second.GetPosition();
    for (auto& enemy : enemies_) {
      auto enemy_pos = enemy.GetPosition();
      float distance = sqrt(pow(tower_pos.first + 0.5 - enemy_pos.first, 2) +
                            pow(tower_pos.second + 0.5 - enemy_pos.second, 2));
      float base_distance =
          sqrt(pow(player_base.first + 0.5 - enemy_pos.first, 2) +
               pow(player_base.second + 0.5 - enemy_pos.second, 2));

      if (distance <= range && base_distance < closest_distance &&
          enemy.IsAlive()) {
        closest_enemy = &enemy;
        closest_distance = base_distance;
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
  if (active_tower_.get_ptr() != 0 && map_(x, y).GetType() == Empty) {
    auto tower = Tower(5, 10, 1, x, y, GetTileSize());
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
  if (gui_.at("sidegui").Get("tower1").Contains(mouse_position)) {
    for (auto& tower : towers_) {
      tower.second.SetInactive();
    }
    active_tower_ =
        Tower(5, 10, 1, mouse_position.x, mouse_position.y, GetTileSize());
    active_tower_->SetActive();
    std::cout << "Pressed Tower1 button" << std::endl;
  } else if (gui_.at("sidegui").Get("nextwave").Contains(mouse_position)) {
    std::cout << "Spawning wave " << wave_ << std::endl;
    AddToSpawnQueue(map_.LoadWave(wave_));
    wave_++;
    gui_.at("sidegui").Get("wave").SetTitle("Wave: " +
                                            std::to_string(wave_ - 1));
  }
}

void PlayState::InitGUI() {
  Gui sidegui = Gui();
  sidegui.Add(
      "tower1",
      GuiEntry(sf::Vector2f(GetTileSize() * map_.GetWidth(), 0), boost::none,
               texture_manager.GetTexture("sprites/basic_tower.png"),
               boost::none));
  sidegui.Add("wave",
              GuiEntry(sf::Vector2f(GetTileSize() * map_.GetWidth(), 150),
                       std::string("Wave: " + std::to_string(wave_ - 1)),
                       boost::none, font_));
  sidegui.Add("player",
              GuiEntry(sf::Vector2f(GetTileSize() * map_.GetWidth(), 250),
                       "Player: " + player_.GetName() +
                           "\nMoney: " + std::to_string(player_.GetMoney()) +
                           "\nLives: " + std::to_string(player_.GetLives()),
                       boost::none, font_));

  sidegui.Add(
      "nextwave",
      GuiEntry(sf::Vector2f(GetTileSize() * map_.GetWidth(), 250),
               std::string("Next wave"),
               texture_manager.GetTexture("sprites/button.png"), font_));

  gui_.insert({"sidegui", sidegui});
}

int PlayState::GetTileSize() const {
  auto windowsize = this->game->window.getSize();
  int tile_size_x = (windowsize.x - 200) / map_.GetWidth();
  int tile_size_y = (windowsize.y) / map_.GetHeight();
  return std::min(tile_size_x, tile_size_y);
}

#include "play_state.hpp"
#include <math.h>
#include <SFML/Graphics.hpp>
#include <boost/format.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include "../configuration/configmanager.hpp"
#include "../enemy/enemy.hpp"
#include "../tower/basic_tower.hpp"
#include "../tower/ship_tower.hpp"
#include "game_state.hpp"
#include "menu_state.hpp"
#include "texturemanager.hpp"

PlayState::PlayState(Game* game, Map map)
    : selected_tower_(nullptr),
      last_spawn_(0),
      wave_(1),
      player_(Player("Pelle")) {
  this->game = game;
  map_ = map;
  sf::Vector2f window_size = sf::Vector2f(this->game->window.getSize());
  sf::View view_(sf::FloatRect(0, 0, window_size.x, window_size.y));
  this->game->window.setView(view_);
  if (!font_.loadFromFile("sprites/Arial.ttf")) {
    std::cout << "Failed to load font";
  }
  background_.setTexture(
      texture_manager.GetTexture("sprites/gui_background.png"));
  background_.setScale(float(this->game->window.getSize().x) /
                           float(background_.getTexture()->getSize().x),
                       float(this->game->window.getSize().y) /
                           float(background_.getTexture()->getSize().y));
  InitGUI();
}

void PlayState::Draw() {
  this->game->window.draw(background_);
  map_.Draw(this->game->window);
  this->game->window.draw(gui_.at("sidegui"));
  int enemies = std::count_if(enemies_.begin(), enemies_.end(),
                              [](Enemy e) { return e.IsAlive(); });
  gui_.at("sidegui").Get("wave").SetTitle(
      "Wave: " + std::to_string(wave_ - 1) +
      "\nEnemies: " + std::to_string(spawn_queue_.size() + enemies));
  if (selected_tower_ != nullptr) this->game->window.draw(gui_.at("towergui"));
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
    tower.second->SetPosition(
        tower.second->GetPosition().first * GetTileSize(),
        tower.second->GetPosition().second * GetTileSize());
    tower.second->SetScale(
        GetTileSize() / (float)(tower.second->GetTexture()).getSize().x,
        GetTileSize() / (float)(tower.second->GetTexture()).getSize().y);
    this->game->window.draw(*tower.second);
  }
  if (active_tower_.get_ptr() != 0) {
    active_tower_->second->SetPosition(
        sf::Mouse::getPosition(this->game->window).x - GetTileSize() / 2,
        sf::Mouse::getPosition(this->game->window).y - GetTileSize() / 2);
    active_tower_->second->SetScale(
        GetTileSize() /
            (float)(active_tower_->second->GetTexture()).getSize().x,
        GetTileSize() /
            (float)(active_tower_->second->GetTexture()).getSize().y);
    this->game->window.draw(*active_tower_.get().second);
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
        int margin = 10;
        int map_size = GetTileSize() * map_.GetWidth();
        gui_.at("sidegui").Get("tower1").SetPosition(sf::Vector2f(map_size, 0));

        int tower_height = gui_.at("sidegui").Get("tower1").GetHeight();

        gui_.at("sidegui").Get("tower2").SetPosition(
            sf::Vector2f(map_size, tower_height + margin));

        tower_height += gui_.at("sidegui").Get("tower2").GetHeight();
        gui_.at("sidegui").Get("wave").SetPosition(
            sf::Vector2f(map_size, tower_height + margin));

        int wave_height = gui_.at("sidegui").Get("wave").GetHeight();
        gui_.at("sidegui").Get("player").SetPosition(
            sf::Vector2f(map_size, tower_height + wave_height + 2 * margin));

        int player_height = gui_.at("sidegui").Get("player").GetHeight();
        gui_.at("sidegui")
            .Get("nextwave")
            .SetPosition(sf::Vector2f(
                map_size,
                tower_height + wave_height + player_height + 3 * margin));

        int nextwave_height = gui_.at("sidegui").Get("nextwave").GetHeight();
        gui_.at("sidegui")
            .Get("cancelbuy")
            .SetPosition(sf::Vector2f(
                map_size, tower_height + wave_height + player_height +
                              nextwave_height + 4 * margin));
        background_.setScale(float(this->game->window.getSize().x) /
                                 float(background_.getTexture()->getSize().x),
                             float(this->game->window.getSize().y) /
                                 float(background_.getTexture()->getSize().y));

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
          UpdatePlayerStats();
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
    if (spawn_queue_.size() == 0) {
      gui_.at("sidegui").Get("nextwave").Show();
    }
  }
}

void PlayState::AddToSpawnQueue(std::vector<Enemy> enemies) {
  for (auto& enemy : enemies) {
    spawn_queue_.push_back(enemy);
  }
  if (spawn_queue_.size() > 0) {
    gui_.at("sidegui").Get("nextwave").Hide();
  } else {
    gui_.at("sidegui").Get("nextwave").Show();
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
    float range = tower.second->GetRange();
    auto tower_pos = tower.second->GetPosition();
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
    if ((closest_enemy) && (cur_time - tower.second->GetLastAttack() >
                            (1 / tower.second->GetAttSpeed()))) {
      tower.second->SetLastAttack(cur_time);
      tower.second->Attack(*closest_enemy);
    }
  }
}

void PlayState::HandleMapClick(int x, int y) {
  // Click on a buildable tile with an active tower
  if (active_tower_.get_ptr() != 0 && map_(x, y).GetType() == Empty &&
      !towers_.count({x, y})) {
    if (active_tower_.get().first == "basic") {
      auto tower = towers_.insert(
          {{x, y},
           std::make_unique<BasicTower>(active_tower_->second->GetRange(),
                                        active_tower_->second->GetDamage(),
                                        active_tower_->second->GetAttSpeed(), x,
                                        y, GetTileSize(),
                                        active_tower_->second->GetPrice())});
      selected_tower_ = tower.first->second.get();
      selected_tower_->SetActive();
      active_tower_ = boost::none;
      gui_.at("sidegui").Get("cancelbuy").Hide();
      InitTowerGUI();
    }
  } else if ((active_tower_.get_ptr() != 0) &&
             (map_(x, y).GetType() == Water1 ||
              map_(x, y).GetType() == Water2) &&
             !towers_.count({x, y})) {
    if (active_tower_.get().first == "ship") {
      auto tower = towers_.insert(
          {{x, y},
           std::make_unique<ShipTower>(active_tower_->second->GetRange(),
                                       active_tower_->second->GetDamage(),
                                       active_tower_->second->GetAttSpeed(), x,
                                       y, GetTileSize(),
                                       active_tower_->second->GetPrice())});
      selected_tower_ = tower.first->second.get();
      selected_tower_->SetActive();
      active_tower_ = boost::none;
      gui_.at("sidegui").Get("cancelbuy").Hide();
      InitTowerGUI();
    }
  }
  // Click on a tower
  else if (towers_.count({x, y}) && active_tower_.get_ptr() == 0) {
    if (selected_tower_ != nullptr) {
      selected_tower_->SetInactive();
    }
    auto tower = towers_.find({x, y});
    selected_tower_ = tower->second.get();
    selected_tower_->SetActive();
    InitTowerGUI();
  }
  // Click on a tile without any towers
  else {
    if (selected_tower_ != nullptr) {
      selected_tower_->SetInactive();
    }
    selected_tower_ = nullptr;
  }
}
void PlayState::HandleGuiClick(sf::Vector2f mouse_position) {
  if (gui_.at("sidegui").Get("tower1").Contains(mouse_position)) {
    for (auto& tower : towers_) {
      tower.second->SetInactive();
    }
    // If we have an selected tower, remove the selection
    if (selected_tower_ != nullptr) selected_tower_ = nullptr;

    auto tower = BasicTower(5, 10, 1, mouse_position.x, mouse_position.y,
                            GetTileSize(), 250);

    // Check if the player has enough money
    if (player_.GetMoney() >= tower.GetPrice()) {
      active_tower_ =
          std::make_pair("basic", std::make_unique<BasicTower>(tower));
      active_tower_->second->SetActive();
      player_.AddMoney(-active_tower_.get().second->GetPrice());
      UpdatePlayerStats();
      gui_.at("sidegui").Get("cancelbuy").Show();
    }
  } else if (gui_.at("sidegui").Get("tower2").Contains(mouse_position)) {
    for (auto& tower : towers_) {
      tower.second->SetInactive();
    }
    // If we have an selected tower, remove the selection
    if (selected_tower_ != nullptr) selected_tower_ = nullptr;

    auto tower = ShipTower(8, 5, 1, mouse_position.x, mouse_position.y,
                           GetTileSize(), 250);

    // Check if the player has enough money
    if (player_.GetMoney() >= tower.GetPrice()) {
      active_tower_ =
          std::make_pair("ship", std::make_unique<ShipTower>(tower));
      active_tower_->second->SetActive();
      player_.AddMoney(-active_tower_.get().second->GetPrice());
      UpdatePlayerStats();
      gui_.at("sidegui").Get("cancelbuy").Show();
    }

  } else if (gui_.at("sidegui").Get("nextwave").IsVisible() &&
             gui_.at("sidegui").Get("nextwave").Contains(mouse_position)) {
    std::cout << "Spawning wave " << wave_ << std::endl;
    AddToSpawnQueue(map_.LoadWave(wave_));
    wave_++;

    int enemies = std::count_if(enemies_.begin(), enemies_.end(),
                                [](Enemy e) { return e.IsAlive(); });

    gui_.at("sidegui").Get("wave").SetTitle(
        "Wave: " + std::to_string(wave_ - 1) +
        "\nEnemies: " + std::to_string(spawn_queue_.size() + enemies));

  } else if (gui_.at("sidegui").Get("cancelbuy").Contains(mouse_position) &&
             active_tower_.get_ptr() != 0) {
    player_.AddMoney(active_tower_.get().second->GetPrice());
    UpdatePlayerStats();
    active_tower_ = boost::none;
    gui_.at("sidegui").Get("cancelbuy").Hide();
  } else if (gui_.find("towergui") != gui_.end() &&
             gui_.at("towergui")
                 .Get("upgrade_tower")
                 .Contains(mouse_position)) {
    selected_tower_->Upgrade();
    UpdateTowerStats();
  } else if (gui_.find("towergui") != gui_.end() &&
             gui_.at("towergui").Get("sell_tower").Contains(mouse_position)) {
    player_.AddMoney(selected_tower_->GetPrice() / 2);
    UpdatePlayerStats();
    towers_.erase(selected_tower_->GetPosition());
    selected_tower_ = nullptr;
  }
}

void PlayState::InitGUI() {
  Gui sidegui = Gui();
  const int margin = 10;
  int map_size = GetTileSize() * map_.GetWidth();
  sidegui.Add("tower1",
              GuiEntry(sf::Vector2f(map_size, 0), boost::none,
                       texture_manager.GetTexture("sprites/basic_tower.png"),
                       boost::none));

  int tower_height = sidegui.Get("tower1").GetHeight();
  int enemies = std::count_if(enemies_.begin(), enemies_.end(),
                              [](Enemy e) { return e.IsAlive(); });

  sidegui.Add(
      "tower2",
      GuiEntry(sf::Vector2f(map_size, tower_height + margin), boost::none,
               texture_manager.GetTexture("sprites/ship_tower.png"),
               boost::none));

  tower_height += sidegui.Get("tower2").GetHeight();

  sidegui.Add(
      "wave",
      GuiEntry(
          sf::Vector2f(map_size, tower_height + margin),
          std::string("Wave: " + std::to_string(wave_ - 1) + "\nEnemies: " +
                      std::to_string(spawn_queue_.size() + enemies)),

          boost::none, font_));
  int wave_height = sidegui.Get("wave").GetHeight();

  sidegui.Add(
      "player",
      GuiEntry(sf::Vector2f(map_size, tower_height + wave_height + 2 * margin),
               "Player: " + player_.GetName() +
                   "\nMoney: " + std::to_string(player_.GetMoney()) +
                   "\nLives: " + std::to_string(player_.GetLives()),
               boost::none, font_));
  int player_height = sidegui.Get("player").GetHeight();
  sidegui.Add(
      "nextwave",
      GuiEntry(sf::Vector2f(map_size, tower_height + wave_height +
                                          player_height + 3 * margin),
               std::string("Next wave"),
               texture_manager.GetTexture("sprites/button.png"), font_));
  int nextwave_height = sidegui.Get("nextwave").GetHeight();

  sidegui.Add(
      "cancelbuy",
      GuiEntry(
          sf::Vector2f(map_size, tower_height + wave_height + player_height +
                                     nextwave_height + 4 * margin),
          std::string("Cancel buy"),
          texture_manager.GetTexture("sprites/button.png"), font_, false));

  gui_.insert({"sidegui", sidegui});
}

void PlayState::InitTowerGUI() {
  Gui towergui = Gui();
  const int margin = 10;
  int map_size = GetTileSize() * map_.GetHeight();
  towergui.Add("tower", GuiEntry(sf::Vector2f(0, map_size), boost::none,
                                 selected_tower_->GetTexture(), boost::none));

  int tower_width = towergui.Get("tower").GetWidth();

  towergui.Add(
      "tower_stats",
      GuiEntry(
          sf::Vector2f(tower_width + margin, map_size),
          "Range: " +
              boost::str(boost::format("%.1f") % selected_tower_->GetRange()) +
              "\nDamage: " +
              boost::str(boost::format("%.1f") % selected_tower_->GetDamage()) +
              "\nAttack speed: " +
              boost::str(boost::format("%.1f") %
                         selected_tower_->GetAttSpeed()),
          boost::none, font_));
  int tower_stats_width = towergui.Get("tower_stats").GetWidth();

  towergui.Add(
      "upgrade_tower",
      GuiEntry(
          sf::Vector2f(tower_width + tower_stats_width + 2 * margin, map_size),
          std::string("Upgrade"),
          texture_manager.GetTexture("sprites/button.png"), font_));
  int upgrade_tower_width = towergui.Get("upgrade_tower").GetWidth();

  towergui.Add(
      "sell_tower",
      GuiEntry(sf::Vector2f(tower_width + tower_stats_width +
                                upgrade_tower_width + 3 * margin,
                            map_size),
               std::string("Sell"),
               texture_manager.GetTexture("sprites/button.png"), font_));

  gui_["towergui"] = towergui;
}

int PlayState::GetTileSize() const {
  auto windowsize = this->game->window.getSize();
  int tile_size_x = (windowsize.x - 200) / map_.GetWidth();
  int tile_size_y = (windowsize.y - 200) / map_.GetHeight();
  return std::min(tile_size_x, tile_size_y);
}

void PlayState::UpdatePlayerStats() {
  gui_.at("sidegui").Get("player").SetTitle(
      "Player: " + player_.GetName() +
      "\nMoney: " + std::to_string(player_.GetMoney()) +
      "\nLives: " + std::to_string(player_.GetLives()));
}

void PlayState::UpdateTowerStats() {
  gui_.at("towergui")
      .Get("tower_stats")
      .SetTitle(
          "Range: " +
          boost::str(boost::format("%.1f") % selected_tower_->GetRange()) +
          "\nDamage: " +
          boost::str(boost::format("%.1f") % selected_tower_->GetDamage()) +
          "\nAttack speed: " +
          boost::str(boost::format("%.1f") % selected_tower_->GetAttSpeed()));
}
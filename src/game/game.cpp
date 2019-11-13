#include "game.hpp"
#include <iostream>

Game::Game() : map_(Map("out/map.txt")), window_(), view_(), gui_() {
  window_.create(sf::VideoMode(800, 600), "Tower Defence");
  gui_.setWindow(window_);
}

void Game::Run() {
  LoadTextures();

  window_.setView(view_);
  window_.setFramerateLimit(60);
  // run the program as long as the window is open
  while (window_.isOpen()) {
    // check all the window's events that were triggered since the last
    // iteration of the loop
    sf::Event event;
    while (window_.pollEvent(event)) {
      // "close requested" event: we close the window
      if (event.type == sf::Event::Closed) window_.close();
      if (event.type == sf::Event::Resized) {
        view_.reset(sf::FloatRect(0, 0, event.size.width, event.size.height));
        window_.setView(view_);
      }
    }
    window_.clear();
    DrawAll();
    window_.display();
  }
}

void Game::DrawAll() {
  DrawMap();
  DrawEnemies();
  DrawTowers();
  DrawGui();
}

void Game::DrawMap() {
  int tile_size = GetTileSize();
  sf::Sprite sprite;
  sf::Texture* texture;

  for (int y = 0; y < map_.GetHeight(); y++) {
    for (int x = 0; x < map_.GetWidth(); x++) {
      Tile tile = map_(x, y);
      texture = &textures_.at(tile.GetTexture());
      sprite.setTexture(*texture);
      sprite.setPosition(x * tile_size, y * tile_size);
      sprite.setScale(tile_size / (float)(*texture).getSize().x,
                      tile_size / (float)(*texture).getSize().y);
      window_.draw(sprite);
    }
  }
}

void Game::DrawEnemies() {
  int enemy_size = GetTileSize();
  Enemy enemy = Enemy(100, 1, 4.5, 2.5);
  sf::Sprite sprite;
  sf::Texture* texture = &textures_.at(enemy.GetTexture());
  sprite.setTexture(*texture);
  sprite.setPosition(enemy.GetPosition().first * enemy_size - enemy_size / 2,
                     enemy.GetPosition().second * enemy_size - enemy_size / 2);
  sprite.setScale(enemy_size / (float)(*texture).getSize().x,
                  enemy_size / (float)(*texture).getSize().y);
  window_.draw(sprite);
}

void Game::DrawTowers() {
  int tower_size = GetTileSize();
  Tower tower = Tower(5, 20, 0, 2);
  sf::Sprite sprite;
  sf::Texture* texture = &textures_.at(tower.GetTexture());
  sprite.setTexture(*texture);
  sprite.setPosition(tower.GetPosition().first * tower_size,
                     tower.GetPosition().second * tower_size);
  sprite.setScale(tower_size / (float)(*texture).getSize().x,
                  tower_size / (float)(*texture).getSize().y);
  window_.draw(sprite);
}

void Game::DrawSidebar() {
  tgui::Grid::Ptr layout = tgui::Grid::create();
  layout->setSize(window_.getSize().x - ((map_.GetWidth() - 1) * GetTileSize()),
                  0.5f * tgui::bindHeight(gui_));
  layout->setPosition((map_.GetWidth() - 1) * GetTileSize(), 0);
  gui_.add(layout);
  sf::Texture* texture = &textures_.at("sprites/basic_tower.png");
  try {
    for (size_t i = 0; i < 2; i++) {
      for (size_t j = 0; j < 2; j++) {
        auto button = tgui::Button::create();
        button->setSize(100, 100);
        // tgui::Texture texturea(*texture, sf::IntRect(0, 0, 0, 0),
        // sf::IntRect(0, 0, 0, 0));

        // button->getRenderer()->setNormalTexture(texturea);

        button->connect("pressed", [&]() { window_.close(); });
        layout->addWidget(button, j, i, tgui::Borders(0, 0, 0, 0));
      }
    }

  } catch (const tgui::Exception& e) {
    std::cerr << "TGUI Exception: " << e.what() << std::endl;
  }
}

void Game::DrawGui() {
  DrawSidebar();
  gui_.draw();
}

void Game::LoadTextures() {
  LoadTexture("sprites/grass_tile_1.png");
  LoadTexture("sprites/sand_tile.png");
  LoadTexture("sprites/basic_enemy.png");
  LoadTexture("sprites/basic_tower.png");
}

void Game::LoadTexture(const std::string texture_name) {
  sf::Texture texture;
  texture.loadFromFile(texture_name);
  texture.setSmooth(true);
  textures_.insert(std::make_pair(texture_name, texture));
}

const int Game::GetTileSize() const {
  auto window_size = window_.getSize();
  int tile_size_x = (window_size.x - 200) / map_.GetWidth();
  int tile_size_y = (window_size.y - 200) / map_.GetHeight();
  return std::min(tile_size_x, tile_size_y);
}
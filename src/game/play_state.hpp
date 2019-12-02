#include <SFML/Graphics.hpp>
#include "../map/map.hpp"
#include "game_state.hpp"

class PlayState : public GameState {
 public:
  PlayState(Game* game);
  virtual void Draw();
  virtual void HandleInput();
  Map map;
  int GetTileSize() const;

 private:
  sf::View view_;
  sf::Sprite background_;
};
#include <SFML/Graphics.hpp>
#include "../map/map.hpp"
#include "game_state.hpp"

class PlayState : public GameState {
 public:
  PlayState(Game* game);
  virtual void Draw();
  virtual void HandleInput();

 private:
  sf::View view_;
  sf::Sprite background_;
  Map map_;
};
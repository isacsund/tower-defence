#include <SFML/Graphics.hpp>
#include <vector>
#include "../gui/button.hpp"
#include "game_state.hpp"

class MenuState : public GameState {
 public:
  MenuState(Game* game);
  virtual void Draw();
  virtual void HandleInput();

 private:
  sf::View view_;
  sf::Sprite background_;
  sf::Font font_;
  std::vector<Button> buttons_;
};
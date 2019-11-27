#include "game/game.hpp"
#include "game/menu_state.hpp"

int main() {
  Game game;
  game.PushState(new MenuState(&game));
  game.Run();
  return 0;
}

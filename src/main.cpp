

#include <iostream>

#include "Basic/game_state.h"
int main() {
  try {
    GameState& gs = GameState::GetGameState();
    gs.StartGame();

  } catch (const std::exception& ex) {
    std::cout << '\n' << ex.what() << '\n';
  }

  return 0;
}

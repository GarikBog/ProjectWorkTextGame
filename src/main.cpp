

#include "Basic/game_state.h"

int main() {
  GameState& gs = GameState::GetGameState();

  gs.StartGame();

  return 0;
}

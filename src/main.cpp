

#include <windows.h>

#include <ctime>
#include <iostream>

#include "Basic/game_state.h"
int main() {
  std::cout << GetACP() << std::endl;
  SetConsoleCP(CP_UTF8);
  SetConsoleOutputCP(CP_UTF8);
  srand(std::time(0));
  setlocale(LC_ALL, "rus");
  try {
    GameState& gs = GameState::GetGameState();
    gs.StartGame();

  } catch (const std::exception& ex) {
    std::cout << '\n' << ex.what() << '\n';
  }

  return 0;
}

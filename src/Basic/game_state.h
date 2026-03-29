
#ifndef GAME_STATE
#define GAME_STATE

#include "../Player/player.h"
#include "../Visual/console.h"

#ifndef STRING
#include <string>
#define STRING
#endif

class GameState {
 public:
  void StartGame();

  static GameState& GetGameState();

  static GameState* game_state_;

 private:
  GameState();
  const unsigned int kWindowWidth_ = 1920;
  const unsigned int kWindowHeight_ = 1080;
  const std::string kConsoleTexturePath = "../textures/console.jpg";

  bool game_is_run_ = true;

  sf::RenderWindow window_;
  Console console_;
  Player player_;

  void Update();
  void Draw();
};

#endif

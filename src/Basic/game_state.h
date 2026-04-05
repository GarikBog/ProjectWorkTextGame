
#ifndef GAME_STATE
#define GAME_STATE

#include "../Player/player.h"
#include "../Visual/console.h"

#ifndef STRING
#include <string>
#define STRING
#endif

#ifndef VECTOR
#define VECTOR
#include <vector>
#endif  // !VECTOR

class MovementController;

class Button;

class BattleField;

class GameState {
 public:
  void StartGame();

  static GameState& GetGameState();

  void PrintText(std::string text, sf::Color color = sf::Color::White);

  void ButtonChangeVisibility(Button* button, bool is_visible);

  sf::RenderWindow& GetWindow();

  Player& GetPlayer();

  void Click();

  ~GameState();

 private:
  static inline GameState* game_state_ = nullptr;
  GameState();
  const unsigned int kWindowWidth_ = 1920;
  const unsigned int kWindowHeight_ = 1080;

  bool game_is_run_ = true;

  sf::RenderWindow window_;
  Console console_;
  Player player_;
  MovementController* movement_controller_;
  float world_delta_time_;

  std::vector<Button*> visible_buttons_;
  BattleField* current_battle_ = nullptr;
  void Update();
  void Draw();

  void AddButton(Button* button);
  void RemoveButton(Button* button);
};

#endif

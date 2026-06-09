
#ifndef GAME_STATE
#define GAME_STATE

#include <windows.h>

#include <map>
#include <string>
#include <vector>

#include "../Player/player.h"
#include "../Story/big_event.h"
#include "../Visual/console.h"
#include "../Visual/ui_inventory.h"
#include "player_behavior.h"

std::wstring StringToWString(const std::string& str);

std::string WStringToString(const std::wstring& wstr);

class MovementController;

class Button;

class BattleField;

class StoryTeller;

class GameState : public IButtonOwner {
 public:
  void StartGame();

  static GameState& GetGameState();

  Console& GetConsole();
  void ButtonChangeVisibility(Button* button, bool is_visible);

  sf::RenderWindow& GetWindow();

  Player& GetPlayer();
  PlayerBehavior& GetPlayerBehavior();
  BigEvent* GetCurrentBigEvent();
  BattleField* GetCurrentBattle();
  MovementController* GetMovementController();

  void Click();
  void RefreshTurn();
  void EndTurn();

  void UpdateUIInvSlots();
  ~GameState();

  void ButtonIsPressed(Button* button) override;

  void PlayerDieInBattle();
  bool IsPlayerAlive() const;

  void LoadBackground(std::string name);

  void LoadNextBigEvent();

  void StartBattle(std::string battle_file_path);

 private:
  static inline const int kBattleButtonX_ = 5;
  static inline const int kBattleButtonY_ = 305;

  static inline const int kInventoryButtonX_ = 5;
  static inline const int kInventoryButtonY_ = 205;

  static inline const int kBackgroundButtonX_ = 5;
  static inline const int kBackgroundButtonY_ = 105;

  static inline const int kSwitchModesButtonW_ = 100;
  static inline const int kSwitchModesButtonH_ = 100;

  static inline const int kBackgroundX_ = 150;
  static inline const int kBackgroundY_ = 30;
  static inline const int kBackgroundW_ = 900;
  static inline const int kBackgroundH_ = 1020;

  static inline GameState* game_state_ = nullptr;
  GameState();
  const unsigned int kWindowWidth_ = 1920;
  const unsigned int kWindowHeight_ = 1080;

  bool game_is_run_ = true;
  bool load_next_big_event = true;
  bool load_new_battle = false;
  std::string new_battle_path;

  sf::RenderWindow window_;
  Console console_;
  Player player_;
  UIInventory* ui_inventory_;
  MovementController* movement_controller_;
  float world_delta_time_;

  Widget background_;
  Button* current_battle_button_;
  Button* ui_inventory_button_;
  Button* background_button_;

  std::vector<Button*> visible_buttons_;
  std::map<std::string, Icon*> backgrounds_images_;

  BattleField* current_battle_ = nullptr;
  BigEvent* current_event_ = nullptr;
  PlayerBehavior player_behavior_;
  StoryTeller* storyteller_;

  void Update();
  void Draw();

  void HideAll();

  void AddButton(Button* button);
  void RemoveButton(Button* button);
  void CleanupBattle();
};

#endif

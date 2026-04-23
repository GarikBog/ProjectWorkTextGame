#ifndef MOVEMENT_CONTROLLER_H_
#define MOVEMENT_CONTROLLER_H_

#include "character.h"
#include "path_data.h"
class BattleField;
class Object;
class Button;
class CharacterInfoWidget;

class MovementController {
 public:
  enum class MovementState { kIdle, kPathPreview, kMoving };

  explicit MovementController(BattleField* battle_field);
  ~MovementController();

  PathResult CalculatePath(int start_x, int start_y, int target_x, int target_y,
                           float speed);

  bool TryMoveToCell(int target_x, int target_y, float speed);
  void Update(float delta_time);
  void ButtonIsPressed(Button* button);

  MovementState GetState() const;
  void SetCurrentCharacter(Object* character);
  Object* GetCurrentCharacter() const;

  void EndTurn();
  void RefreshTurn();
  void StartNextNPC();

  void FindAllNPCs();
  void SetInfoWidget(CharacterInfoWidget* widget);

  void UnregisterObject(Object* object);

 private:
  float GetCellCost(int x, int y);
  void HighlightPath(const PathResult& path);
  void ClearHighlights();
  void StartMoving(const PathResult& path);

  BattleField* battle_field_;
  MovementState state_;
  Object* current_character_;
  CharacterInfoWidget* info_widget_;
  int start_x_;
  int start_y_;
  int target_x_;
  int target_y_;
  PathResult current_path_;
  int current_step_index_;
  float step_timer_;
  bool is_player_turn_;
  std::vector<Character*> npc_characters_;
  int current_npc_index_;
  PathResult npc_current_path_;
  bool is_npc_moving_;
  Character* current_moving_npc_;
};

#endif  // MOVEMENT_CONTROLLER_H_

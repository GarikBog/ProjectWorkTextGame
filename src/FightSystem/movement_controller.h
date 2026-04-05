#ifndef MOVEMENT_CONTROLLER_H_
#define MOVEMENT_CONTROLLER_H_

#include "path_data.h"

class BattleField;
class Object;
class Button;

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

 private:
  float GetCellCost(int x, int y);
  void HighlightPath(const PathResult& path);
  void ClearHighlights();
  void StartMoving(const PathResult& path);

  BattleField* battle_field_;
  MovementState state_;
  Object* current_character_;
  int start_x_;
  int start_y_;
  int target_x_;
  int target_y_;
  PathResult current_path_;
  float total_speed_;
  int current_step_index_;
  float step_timer_;
};

#endif  // MOVEMENT_CONTROLLER_H_

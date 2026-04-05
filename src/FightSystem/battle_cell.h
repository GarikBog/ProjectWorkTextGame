#ifndef BATTLE_CELL
#define BATTLE_CELL

#include "../Basic/i_button_owner.h"
#include "../Visual/button.h"
#include "../Visual/widget.h"
#include "object.h"

class MovementController;  // Forward declaration

class BattleCell : public IButtonOwner {
 public:
  ~BattleCell();
  void SetObjectOnCell(Object* object);
  bool ObjectMovesToCell(Object* object);
  bool ObjectMovesFromCell(Object* object);
  Object* GetObjectOnCell();

  void SetHighlight(const sf::Color& color);
  void ClearHighlight();

  BattleCell(Object& basic_object, Object* tmp_object, sf::RenderWindow& window,
             const std::pair<float, float> pos = {0, 0},
             const std::pair<int, int> scale = {0, 0});
  void ButtonIsPressed(Button* button) override;
  Button& GetButton();
  void Draw();
  void SetMovementController(MovementController* controller);

 private:
  Object* tmp_object_on_cell_;
  Object& basic_object_on_cell_;
  Button cell_button_;

  sf::Color original_color_;
  bool has_highlight_;

  MovementController* movement_controller_;
};

#endif  // BATTLE_CELL

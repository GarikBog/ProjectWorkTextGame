#include "battle_cell.h"

#include "../Basic/game_state.h"
#include "movement_controller.h"  // Добавить

BattleCell::~BattleCell() {}

void BattleCell::SetObjectOnCell(Object* object) {
  tmp_object_on_cell_ = object;
  if (tmp_object_on_cell_ != nullptr) {
    cell_button_.SetIcon(tmp_object_on_cell_->GetIcon());
  } else {
    cell_button_.SetIcon(basic_object_on_cell_.GetIcon());
  }
}

bool BattleCell::ObjectMovesToCell(Object* object) {
  if (tmp_object_on_cell_ && tmp_object_on_cell_->StayOn(object)) return true;
  if (!tmp_object_on_cell_ && basic_object_on_cell_.StayOn(object)) return true;
  return false;
}

bool BattleCell::ObjectMovesFromCell(Object* object) {
  if (tmp_object_on_cell_ && tmp_object_on_cell_->MoveFrom(object)) return true;

  if (!tmp_object_on_cell_ && basic_object_on_cell_.MoveFrom(object))
    return true;
  return false;
}

Object* BattleCell::GetObjectOnCell() { return tmp_object_on_cell_; }

void BattleCell::Draw() { cell_button_.Draw(); }

BattleCell::BattleCell(Object& basic_object, Object* tmp_object,
                       sf::RenderWindow& window,
                       const std::pair<float, float> pos,
                       const std::pair<int, int> scale)
    : tmp_object_on_cell_(tmp_object),
      cell_button_(this, window, basic_object.GetIcon(), pos, scale),
      basic_object_on_cell_(basic_object),
      has_highlight_(false),
      movement_controller_(nullptr) {
  if (tmp_object_on_cell_) cell_button_.SetIcon(tmp_object_on_cell_->GetIcon());
}

void BattleCell::ButtonIsPressed(Button* button) {
  if (movement_controller_ != nullptr) {
    movement_controller_->ButtonIsPressed(button);
  }
}

Button& BattleCell::GetButton() { return cell_button_; }

void BattleCell::SetHighlight(const sf::Color& color) {
  if (!has_highlight_) {
    original_color_ = cell_button_.GetColor();
    has_highlight_ = true;
  }
  cell_button_.SetColor(color);
}

void BattleCell::ClearHighlight() {
  if (has_highlight_) {
    cell_button_.SetColor(original_color_);
    has_highlight_ = false;
  }
}

void BattleCell::SetMovementController(MovementController* controller) {
  movement_controller_ = controller;
}

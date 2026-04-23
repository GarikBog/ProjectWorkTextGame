#include "body_part_widget.h"

#include "../Basic/game_state.h"
#include "../FightSystem/battle_field.h"
#include "../FightSystem/movement_controller.h"
#include "button.h"
BodyPartWidget::BodyPartWidget(sf::RenderWindow& window,
                               const std::pair<float, float> pos,
                               const std::pair<int, int> scale,
                               const std::string texture_file,
                               const sf::IntRect texture_rectangle,
                               std::vector<BodyPart*> parts)
    : Widget(window, pos, scale, texture_file, texture_rectangle) {
  SetParts(parts);
}

void BodyPartWidget::SetParts(std::vector<BodyPart*> new_parts) {
  parts_.clear();
  parts_buttons_.clear();

  for (BodyPart* part : new_parts) {
    sf::IntRect rectangle = part->GetIcon().GetTextureRect();
    parts_.push_back(part);
    parts_buttons_.push_back(
        Button(static_cast<IButtonOwner*>(this), window_, &part->GetIcon(),
               std::pair(x_ + rectangle.left, y_ + rectangle.top),
               std::pair<int, int>(rectangle.width, rectangle.height)));
  }
}

void BodyPartWidget::ButtonIsPressed(Button* button) {
  for (int i = 0; i < parts_buttons_.size(); ++i) {
    if (&parts_buttons_[i] == button) {
      GameState::GetGameState().GetCurrentBattle()->TryAttackCharacter(
          GameState::GetGameState().GetPlayer().GetPlayerCharacter(),
          dynamic_cast<Character*>(GameState::GetGameState()
                                       .GetMovementController()
                                       ->GetCurrentCharacter()),
          parts_[i]);
    }
  }
}

void BodyPartWidget::Draw() {
  Widget::Draw();

  for (Button& button : parts_buttons_) {
    button.Draw();
  }
}

void BodyPartWidget::SetVisibility(bool is_visible_now) {
  is_visible_ = is_visible_now;
  for (Button& button : parts_buttons_) {
    button.SetVisibility(is_visible_now);
  }
}

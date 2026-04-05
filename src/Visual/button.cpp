#include "button.h"

#include "../Basic/game_state.h"
#include "../Basic/i_button_owner.h"
bool Button::Click(const sf::Vector2i& mouse_pos) {
  if (CheckMouseBorders(mouse_pos)) {
    owner_.ButtonIsPressed(this);
    return true;
  };
  return false;
}

Button::Button(IButtonOwner* owner, sf::RenderWindow& window,
               const std::pair<float, float> pos,
               const std::pair<int, int> scale, const std::string texture_file,
               const sf::IntRect texture_rectangle)
    : owner_(*owner),
      Widget(window, pos, scale, texture_file, texture_rectangle) {
  SetVisibility(true);
}

Button::Button(IButtonOwner* owner, sf::RenderWindow& window, Icon* icon,
               const std::pair<float, float> pos,
               const std::pair<int, int> scale)
    : owner_(*owner), Widget(window, icon, pos, scale) {
  SetVisibility(true);
}
Button::~Button() {
  GameState::GetGameState().ButtonChangeVisibility(this, false);
}

void Button::SetVisibility(bool is_hidden_now) {
  Widget::SetVisibility(is_hidden_now);
  GameState::GetGameState().ButtonChangeVisibility(this, is_hidden_now);
}

void Button::SetIcon(Icon* icon) { Widget::SetIcon(icon); }

Icon* Button::GetIcon() const { return icon_; }

void Button::SetColor(const sf::Color& color) { sprite_.setColor(color); }

sf::Color Button::GetColor() const { return sprite_.getColor(); }

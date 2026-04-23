#include "character_info_widget.h"

#include "../Basic/game_state.h"
#include "../FightSystem/battle_field.h"
#include "../FightSystem/character.h"
CharacterInfoWidget::CharacterInfoWidget(sf::RenderWindow& window,
                                         const std::pair<float, float> pos,
                                         const std::pair<int, int> scale)
    : Widget(window, pos, scale, "", {0, 0, 0, 0}),
      current_object_(nullptr),
      attack_button_(
          static_cast<IButtonOwner*>(this), window,
          std::pair(pos.first + scale.first - kActionButtonRightOffset -
                        kActionButtonWidth,
                    pos.second + kIconMargin),
          std::pair(kActionButtonWidth, kActionButtonHeight),
          ASSETS_PATH "/textures/buttons/Attack.png",
          sf::IntRect(0, 0, 500, 200)),
      check_button_(
          static_cast<IButtonOwner*>(this), window,
          std::pair(pos.first + scale.first - kActionButtonRightOffset -
                        kActionButtonWidth,
                    pos.second + kIconMargin + kActionButtonBetweenOffset +
                        kActionButtonHeight),
          std::pair(kActionButtonWidth, kActionButtonHeight),
          ASSETS_PATH "/textures/buttons/Check.png",
          sf::IntRect(0, 0, 500, 200)) {
  SetBackgroundColor(sf::Color(30, 30, 30, 230));
  SetOutlineColor(sf::Color(150, 150, 150));
  SetOutlineThickness(2.0f);

  if (!font_.loadFromFile(ASSETS_PATH "/fonts/CorrectionBrush.otf")) {
    throw std::exception("Could not open font for InfoWidget");
  }

  icon_border_.setFillColor(sf::Color(20, 20, 20, 255));
  icon_border_.setOutlineColor(sf::Color(200, 200, 200));
  icon_border_.setOutlineThickness(2.0f);

  name_text_.setFont(font_);
  name_text_.setCharacterSize(kNameFontSize);
  name_text_.setFillColor(sf::Color::White);

  health_text_.setFont(font_);
  health_text_.setCharacterSize(kStatsFontSize);
  health_text_.setFillColor(sf::Color::Green);

  stamina_text_.setFont(font_);
  stamina_text_.setCharacterSize(kStatsFontSize);
  stamina_text_.setFillColor(sf::Color::Cyan);

  SetVisibility(true);
}

Object* CharacterInfoWidget::GetCurrentObject() { return current_object_; }

void CharacterInfoWidget::SetObject(Object* character) {
  current_object_ = character;
  UpdateIcon();
  UpdateText();
  SetVisibility(true);
  if (character->GetType() == kPlayerCharacter) {
    attack_button_.SetVisibility(false);
    check_button_.SetVisibility(false);
  } else {
    attack_button_.SetVisibility(true);
    check_button_.SetVisibility(true);
  }
}

void CharacterInfoWidget::ClearCharacter() {
  current_object_ = nullptr;
  SetVisibility(false);
}

void CharacterInfoWidget::UpdateIcon() {
  if (current_object_ == nullptr) return;

  Icon* icon = current_object_->GetIcon();
  if (icon != nullptr) {
    character_sprite_.setTexture(icon->GetTexture());
    character_sprite_.setTextureRect(icon->GetTextureRect());

    float available_size = static_cast<float>(GetHeight()) - 2 * kIconMargin;
    float icon_size = std::min(available_size, kIconSize);

    float scale_x = icon_size / character_sprite_.getLocalBounds().width;
    float scale_y = icon_size / character_sprite_.getLocalBounds().height;
    character_sprite_.setScale(scale_x, scale_y);

    float actual_width = character_sprite_.getLocalBounds().width * scale_x;
    float actual_height = character_sprite_.getLocalBounds().height * scale_y;
    icon_border_.setSize(sf::Vector2f(actual_width, actual_height));
  }
}

void CharacterInfoWidget::UpdateText() {
  if (current_object_ == nullptr) return;

  float widget_x = GetX();
  float widget_y = GetY();
  float widget_width = static_cast<float>(GetWidth());

  float icon_right = kIconMargin + icon_border_.getSize().x;
  float text_start_x = widget_x + icon_right + kTextMargin;

  float max_text_width = widget_width - icon_right - kTextMargin - kIconMargin -
                         kActionButtonWidth - kActionButtonRightOffset;

  name_text_.setString(current_object_->GetName());
  name_text_.setPosition(text_start_x, widget_y + kNameYOffset);
  Character* current_character_ = dynamic_cast<Character*>(current_object_);
  if (current_character_) {
    BattleStats& stats = current_character_->GetStats();
    health_text_.setString("Health: " + std::to_string(stats.health) + "/" +
                           std::to_string(stats.max_health));
    health_text_.setPosition(text_start_x, widget_y + kHealthYOffset);

    stamina_text_.setString(
        "Stamina: " + std::to_string(stats.current_stamina) + "/" +
        std::to_string(stats.stamina));
    stamina_text_.setPosition(text_start_x, widget_y + kStaminaYOffset);
  } else {
    health_text_.setString("");
    stamina_text_.setString("");
  }
}

void CharacterInfoWidget::ButtonIsPressed(Button* button) {
  if (button == &attack_button_) {
    GameState::GetGameState().GetCurrentBattle()->TryAttackObject(
        GameState::GetGameState().GetPlayer().GetPlayerCharacter(),
        current_object_);
  } else if (button == &check_button_) {
    current_object_->Check();
  }
}

void CharacterInfoWidget::Draw() {
  if (!is_visible_) return;
  Widget::Draw();
  if (current_object_ != nullptr && is_visible_) {
    float widget_x = GetX();
    float widget_y = GetY();

    float icon_size = icon_border_.getSize().x;
    icon_border_.setPosition(widget_x + kIconMargin, widget_y + kIconMargin);
    character_sprite_.setPosition(widget_x + kIconMargin,
                                  widget_y + kIconMargin);

    float icon_right = kIconMargin + icon_size;
    float text_start_x = widget_x + icon_right + kTextMargin;

    name_text_.setPosition(text_start_x, widget_y + kNameYOffset);
    health_text_.setPosition(text_start_x, widget_y + kHealthYOffset);
    stamina_text_.setPosition(text_start_x, widget_y + kStaminaYOffset);
  }

  if (current_object_ != nullptr && is_visible_) {
    window_.draw(icon_border_);
    window_.draw(character_sprite_);
    window_.draw(name_text_);
    window_.draw(health_text_);
    window_.draw(stamina_text_);
    attack_button_.Draw();
    check_button_.Draw();
  }
}

void CharacterInfoWidget::SetVisibility(bool is_visible_now) {
  Widget::SetVisibility(is_visible_now);
}

void CharacterInfoWidget::UnregisterObject(Object* object) {
  if (object == nullptr) return;

  if (current_object_ == object) {
    ClearCharacter();
  }
}

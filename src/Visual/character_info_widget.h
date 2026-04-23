#ifndef CHARACTER_INFO_WIDGET
#define CHARACTER_INFO_WIDGET

#include "../Basic/i_button_owner.h"
#include "../Visual/button.h"
#include "../Visual/widget.h"

class Character;

class Object;

class CharacterInfoWidget : public Widget, public IButtonOwner {
 public:
  CharacterInfoWidget(sf::RenderWindow& window,
                      const std::pair<float, float> pos,
                      const std::pair<int, int> scale);
  Object* GetCurrentObject();
  void SetObject(Object* character);
  void ClearCharacter();
  void Draw() override;
  void SetVisibility(bool is_visible_now) override;

  void UpdateText();
  void ButtonIsPressed(Button* button) override;

  void UnregisterObject(Object* object);
 private:
  // Константы отступов и размеров
  static constexpr float kIconSize = 96.0f;
  static constexpr float kIconMargin = 10.0f;
  static constexpr float kTextMargin = 15.0f;
  static constexpr float kLineHeight = 25.0f;
  static constexpr float kActionButtonWidth = 152.f;
  static constexpr float kActionButtonRightOffset = 10.f;
  static constexpr float kActionButtonHeight = 38.f;
  static constexpr float kActionButtonBetweenOffset = 20.f;
  static constexpr float kNameYOffset = 15.0f;
  static constexpr float kHealthYOffset = 45.0f;
  static constexpr float kStaminaYOffset = 75.0f;
  static constexpr int kNameFontSize = 20;
  static constexpr int kStatsFontSize = 16;

  Object* current_object_;
  Button attack_button_;
  Button check_button_;

  sf::Sprite character_sprite_;
  sf::RectangleShape icon_border_;
  sf::Text name_text_;
  sf::Text health_text_;
  sf::Text stamina_text_;
  sf::Font font_;

  void UpdateIcon();
};

#endif

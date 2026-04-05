
#ifndef BUTTON
#define BUTTON
#include "widget.h"

class IButtonOwner;

class Button : public Widget {
 public:
  bool Click(const sf::Vector2i& mouse_pos);

  Button(IButtonOwner* owner, sf::RenderWindow& window,
         const std::pair<float, float> pos = {0, 0},
         const std::pair<int, int> scale = {0, 0},
         const std::string texture_file = "",
         const sf::IntRect texture_rectangle = {0, 0, 0, 0});
  Button(IButtonOwner* owner, sf::RenderWindow& window, Icon* icon = nullptr,
         const std::pair<float, float> pos = {0, 0},
         const std::pair<int, int> scale = {0, 0});
  ~Button();
  void SetVisibility(bool is_visible_now) override;

  void SetIcon(Icon* icon);
  Icon* GetIcon() const;
  void SetColor(const sf::Color& color);
  sf::Color GetColor() const;

 private:
  IButtonOwner& owner_;
};
#endif  // !BUTTON

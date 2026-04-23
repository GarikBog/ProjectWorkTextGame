#ifndef BODY_PART_WIDGET
#define BODY_PART_WIDGET

#include "../Basic/i_button_owner.h"
#include "../FightSystem/body_part.h"
#include "widget.h"

class BodyPartWidget : public Widget, public IButtonOwner {
 public:
  BodyPartWidget(sf::RenderWindow& window,
                 const std::pair<float, float> pos = {0, 0},
                 const std::pair<int, int> scale = {0, 0},
                 const std::string texture_file = "",
                 const sf::IntRect texture_rectangle = {0, 0, 0, 0},
                 std::vector<BodyPart*> parts = {});
  void SetParts(std::vector<BodyPart*> new_parts);

  void ButtonIsPressed(Button* button) override;

  void Draw() override;
  void SetVisibility(bool is_visible_now) override;

 private:
  std::vector<BodyPart*> parts_;
  std::vector<Button> parts_buttons_;
};
#endif  // !BODY_PART_WIDGET

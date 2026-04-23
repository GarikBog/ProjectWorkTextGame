#ifndef UI_INVENTORY
#define UI_INVENTORY

#include "../Basic/i_button_owner.h"
#include "../Player/inventory.h"
#include "button.h"

class UIInventory : public Widget, public IButtonOwner {
 public:
  UIInventory(sf::RenderWindow& window,
              const std::pair<float, float> pos = {0, 0},
              const std::string texture_file = "",
              const sf::IntRect texture_rectangle = {0, 0, 0, 0});

  void UpdateSlots();

  void ButtonIsPressed(Button* button) override;

  void Draw() override;

  void SetVisibility(bool is_visible_now) override;

 private:
  static inline const int kUIInventoryWidth = 900;
  static inline const int kUIInventoryHeight = 900;
  static inline const int kUIInventoryGridWidth = 540;
  static inline const int kUIInventoryGridHeight = 750;
  static inline const int kUIInventoryTopOffset = 10;
  static inline const int kUIInventoryRightOffset = 10;
  static inline const int kRowCount_ = 5;
  static inline const int kColumsCount_ = 7;
  static inline const int kBorderSlotMargin_ = 10;
  static inline const int kBetweenSlotMargin_ = 5;
  float slot_size_;

  static inline const float kHelmetSlotX_ = 110;
  static inline const float kHelmetSlotY_ = 50;
  static inline const float kHelmetSlotW_ = 130;
  static inline const float kHelmetSlotH_ = 130;

  static inline const float kHandSlotX_ = 12.5;
  static inline const float kHandSlotY_ = 240;
  static inline const float kHandSlotW_ = 150;
  static inline const float kHandSlotH_ = 250;

  static inline const float kArmorSlotX_ = 187.5;
  static inline const float kArmorSlotY_ = 240;
  static inline const float kArmorSlotW_ = 150;
  static inline const float kArmorSlotH_ = 250;

  static inline const float kBeltSlotX_ = 5;
  static inline const float kBeltSlotY_ = 560;
  static inline const float kBeltSlotW_ = 340;
  static inline const float kBeltSlotH_ = 72;
  static inline const float kBetweenBeltSlotMargin_ = 5;
  static inline const float kHeightBorderSlotMargin_ = 5;
  static inline const float kWidthBorderSlotMargin_ = 5;
  float belt_slot_size_;

  static inline const float kNextPageButtonSlotX_ = 740;
  static inline const float kPrevPageButtonSlotX_ = 350;
  static inline const float kSwitchPageButtonSlotY_ = 760;
  static inline const float kSwitchPageButtonSlotW_ = 150;
  static inline const float kSwitchPageButtonSlotH_ = 70;

  static inline const int StatsTextSize_ = 26;
  static inline const int kStatsTextX_ = 5;
  static inline const int kStatsTextY_ = 650;

  std::vector<std::vector<Button>> slots_;
  Button* belt_[BELT_COUNT];
  Button next_page_button_;
  Button prev_page_button_;
  Button hand_slot_button_;
  Button armor_slot_button_;
  Button helmet_slot_button_;
  std::pair<int, int> selected_slot_;
  bool is_slot_selected_;
  Inventory& player_invenotory_;
  Icon base_slot_icon_;

  int max_page = 0;
  int current_page = 0;

  sf::Text battle_stats_text_;
  sf::Font battle_stats_font_;

  void SelectSlot(std::pair<int, int> slot);
  void DeselectAll();

  void CreateSlots();
  void GoNextPage();
  void GoPrevPage();

  int GetItemPosFrom2DCoords(std::pair<int, int> pos) const;
  int GetItemPosFrom2DCoords(int first, int second) const;
  std::pair<int, int> Get2DCoordsFromItemPos(int pos) const;
};

#endif  // !UI_INVENTORY

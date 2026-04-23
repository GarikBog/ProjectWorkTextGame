#include "ui_inventory.h"

#include "../Basic/game_state.h"

UIInventory::UIInventory(sf::RenderWindow& window,
                         const std::pair<float, float> pos,
                         const std::string texture_file,
                         const sf::IntRect texture_rectangle)
    : Widget(window, pos, std::pair(kUIInventoryWidth, kUIInventoryHeight),
             texture_file, texture_rectangle),
      player_invenotory_(GameState::GetGameState().GetPlayer().GetInventory()),
      is_slot_selected_(false),
      selected_slot_(-1, -1),
      base_slot_icon_(ASSETS_PATH "/textures/buttons/InvSlot.png",
                      sf::IntRect(0, 0, 200, 200)),
      hand_slot_button_(this, window, &base_slot_icon_,
                        {x_ + kHandSlotX_, y_ + kHandSlotY_},
                        {kHandSlotW_, kHandSlotH_}),
      armor_slot_button_(this, window, &base_slot_icon_,
                         {x_ + kArmorSlotX_, y_ + kArmorSlotY_},
                         {kArmorSlotW_, kArmorSlotH_}),
      helmet_slot_button_(this, window, &base_slot_icon_,
                          {x_ + kHelmetSlotX_, y_ + kHelmetSlotY_},
                          {kHelmetSlotW_, kHelmetSlotH_}),
      next_page_button_(
          this, window,
          std::pair(x_ + kNextPageButtonSlotX_, y_ + kSwitchPageButtonSlotY_),
          std::pair(kSwitchPageButtonSlotW_, kSwitchPageButtonSlotH_),
          ASSETS_PATH "/textures/buttons/next.png",
          sf::IntRect(0, 0, kSwitchPageButtonSlotW_, kSwitchPageButtonSlotH_)),
      prev_page_button_(
          this, window,
          std::pair(x_ + kPrevPageButtonSlotX_, y_ + kSwitchPageButtonSlotY_),
          std::pair(kSwitchPageButtonSlotW_, kSwitchPageButtonSlotH_),
          ASSETS_PATH "/textures/buttons/prev.png",
          sf::IntRect(0, 0, kSwitchPageButtonSlotW_, kSwitchPageButtonSlotH_)) {
  slot_size_ =
      std::min((kUIInventoryGridWidth - kBorderSlotMargin_ * 2 -  // slot size
                (kRowCount_ - 1) * kBetweenSlotMargin_) /
                   kRowCount_,
               (kUIInventoryGridHeight - kBorderSlotMargin_ * 2 -
                (kColumsCount_ - 1) * kBetweenSlotMargin_) /
                   kColumsCount_);
  belt_slot_size_ =
      std::min((kBeltSlotW_ - kWidthBorderSlotMargin_ * 2 -  // belt slot size
                kBetweenBeltSlotMargin_ * (BELT_COUNT - 1)) /
                   BELT_COUNT,
               (kBeltSlotH_ - kHeightBorderSlotMargin_ * 2));
  for (int i = 0; i < BELT_COUNT; ++i) {  // belit init
    belt_[i] = new Button(
        this, window_, &base_slot_icon_,
        std::pair(x_ + kBeltSlotX_ + kWidthBorderSlotMargin_ +
                      (belt_slot_size_ + kBetweenBeltSlotMargin_) * i,
                  y_ + kBeltSlotY_ + kHeightBorderSlotMargin_),
        std::pair(belt_slot_size_, belt_slot_size_));
  }

  // Загружаем шрифт
  if (!battle_stats_font_.loadFromFile(ASSETS_PATH
                                       "/fonts/CorrectionBrush.otf"))
    throw std::exception("Can't open battle sz  tats font!");

  battle_stats_text_.setFont(battle_stats_font_);  // Используем член класса
  battle_stats_text_.setCharacterSize(
      StatsTextSize_);  // Установите нужный размер
  battle_stats_text_.setPosition(x_ + kStatsTextX_, y_ + kStatsTextY_);
  battle_stats_text_.setFillColor(
      sf::Color::Black);  // Добавьте цвет для видимости

  CreateSlots();
  UpdateSlots();
}

void UIInventory::UpdateSlots() {
  const int total_slots = kRowCount_ * kColumsCount_;
  const int items_count = player_invenotory_.GetItemCount();

  max_page = (items_count + total_slots - 1) / total_slots;

  // Обновление ячеек инвентаря
  for (int i = 0 + current_page * total_slots;
       i < total_slots * (current_page + 1); ++i) {
    auto pos = Get2DCoordsFromItemPos(i);
    Icon* icon_to_set = &base_slot_icon_;

    if (i < items_count) {
      Icon* item_icon = player_invenotory_.GetItemIcon(i);
      if (item_icon) {
        icon_to_set = item_icon;
      }
    }
    slots_[pos.first][pos.second].SetIcon(icon_to_set);
  }

  // Обновление иконки шлема
  Icon* helmet_icon = player_invenotory_.GetHelmetItemIcon();
  if (!helmet_icon)
    helmet_slot_button_.SetIcon(&base_slot_icon_);
  else if (helmet_icon != helmet_slot_button_.GetIcon()) {
    helmet_slot_button_.SetIcon(helmet_icon);
  }

  // Обновление иконки рук
  Icon* hand_icon = player_invenotory_.GetHandItemIcon();
  if (!hand_icon)
    hand_slot_button_.SetIcon(&base_slot_icon_);
  else if (hand_icon != hand_slot_button_.GetIcon()) {
    hand_slot_button_.SetIcon(hand_icon);
  }

  // Обновление иконки брони
  Icon* armor_icon = player_invenotory_.GetArmorItemIcon();
  if (!armor_icon)
    armor_slot_button_.SetIcon(&base_slot_icon_);
  else if (armor_icon != armor_slot_button_.GetIcon()) {
    armor_slot_button_.SetIcon(armor_icon);
  }

  // Обновление иконок пояса
  for (int i = 0; i < BELT_COUNT; ++i) {
    Icon* belt_icon = player_invenotory_.GetBeltItemIcon(i);
    if (!belt_icon)
      belt_[i]->SetIcon(&base_slot_icon_);
    else if (belt_icon != belt_[i]->GetIcon()) {
      belt_[i]->SetIcon(belt_icon);
    }
  }

  // Обновление текста статов
  battle_stats_text_.setString(GameState::GetGameState()
                                   .GetPlayer()
                                   .GetBattleStats()
                                   .GetStatsInString());
}
void UIInventory::ButtonIsPressed(Button* button) {
  if (button == &next_page_button_) {
    GoNextPage();
  } else if (button == &prev_page_button_) {
    GoPrevPage();
  } else

      if (button == &helmet_slot_button_) {
    if (is_slot_selected_)
      player_invenotory_.AddItemToHelmetSlot(
          GetItemPosFrom2DCoords(selected_slot_));
    else
      player_invenotory_.AddItemToHelmetSlot(nullptr);
    DeselectAll();
  }  // Helmet
  else if (button == &hand_slot_button_) {
    if (is_slot_selected_)
      player_invenotory_.AddItemToHandSlot(
          GetItemPosFrom2DCoords(selected_slot_));
    else
      player_invenotory_.AddItemToHandSlot(nullptr);
    DeselectAll();
  }  // Hand Slot
  else if (button == &armor_slot_button_) {
    if (is_slot_selected_)
      player_invenotory_.AddItemToArmorSlot(
          GetItemPosFrom2DCoords(selected_slot_));
    else
      player_invenotory_.AddItemToArmorSlot(nullptr);
    DeselectAll();

  } else {  // Hand Slot
    for (int i = 0; i < BELT_COUNT; ++i) {
      if (button == belt_[i]) {
        if (is_slot_selected_)
          player_invenotory_.AddItemToBelt(
              GetItemPosFrom2DCoords(selected_slot_), i);
        else
          player_invenotory_.AddItemToBelt(nullptr, i);
        DeselectAll();

        goto exit_loops;
      }
    }
    // Selecting Inventory Slots
    for (int i = 0; i < slots_.size(); ++i)
      for (int j = 0; j < slots_[i].size(); ++j) {
        if (&slots_[i][j] == button) {
          SelectSlot(std::pair(i, j));
          goto exit_loops;
        }
      }
  }

exit_loops:
  UpdateSlots();
}

void UIInventory::Draw() {
  if (!is_visible_) return;
  Widget::Draw();

  helmet_slot_button_.Draw();
  hand_slot_button_.Draw();
  armor_slot_button_.Draw();

  next_page_button_.Draw();
  prev_page_button_.Draw();

  for (int i = 0; i < BELT_COUNT; ++i) {
    belt_[i]->Draw();
  }

  for (std::vector<Button> row : slots_)
    for (Button& button : row) {
      button.Draw();
    }

  window_.draw(battle_stats_text_);
}

void UIInventory::SetVisibility(bool is_visible_now) {
  Widget::SetVisibility(is_visible_now);

  helmet_slot_button_.SetVisibility(is_visible_now);
  hand_slot_button_.SetVisibility(is_visible_now);
  armor_slot_button_.SetVisibility(is_visible_now);

  next_page_button_.SetVisibility(is_visible_now);
  prev_page_button_.SetVisibility(is_visible_now);

  for (int i = 0; i < BELT_COUNT; ++i) {
    if (belt_[i]) {
      belt_[i]->SetVisibility(is_visible_now);
    }
  }

  for (auto& row : slots_) {
    for (auto& button : row) {
      button.SetVisibility(is_visible_now);
    }
  }
}

void UIInventory::SelectSlot(std::pair<int, int> slot) {
  if (selected_slot_ == slot) {
    DeselectAll();
    return;
  }

  DeselectAll();
  selected_slot_ = slot;
  slots_[slot.first][slot.second].SetColor(sf::Color::Green);
  is_slot_selected_ = true;
}

void UIInventory::DeselectAll() {
  if (!is_slot_selected_) return;
  slots_[selected_slot_.first][selected_slot_.second].SetColor(
      sf::Color::White);
  selected_slot_ = std::pair(-1, -1);
  is_slot_selected_ = false;
  return;
}

void UIInventory::CreateSlots() {
  int items_count = player_invenotory_.GetItemCount();
  slots_.resize(kRowCount_);
  for (int i = 0; i < kRowCount_; ++i) {
    slots_[i].reserve(kColumsCount_);
    for (int j = 0; j < kColumsCount_; ++j) {
      slots_[i].push_back(std::move(
          Button(this, window_, &base_slot_icon_,
                 std::pair(x_ +
                               (kUIInventoryWidth - kUIInventoryGridWidth -
                                kUIInventoryRightOffset) +
                               kBorderSlotMargin_ +
                               i * (slot_size_ + kBetweenSlotMargin_),
                           y_ + kBorderSlotMargin_ + kUIInventoryTopOffset +
                               j * (slot_size_ + kBetweenSlotMargin_)),
                 std::pair(slot_size_, slot_size_))));
    }
  }
}

void UIInventory::GoNextPage() {
  if (current_page + 1 >= max_page) return;

  ++current_page;
}

void UIInventory::GoPrevPage() {
  if (current_page - 1 < 0) return;

  --current_page;
}

std::pair<int, int> UIInventory::Get2DCoordsFromItemPos(int pos) const {
  const int items_per_page = kRowCount_ * kColumsCount_;
  const int local_pos = pos - current_page * items_per_page;

  return std::pair<int, int>(local_pos / kColumsCount_,
                             local_pos % kColumsCount_);
}
int UIInventory::GetItemPosFrom2DCoords(std::pair<int, int> pos) const {
  return pos.first * kColumsCount_ + pos.second +
         current_page * kRowCount_ * kColumsCount_;
}

int UIInventory::GetItemPosFrom2DCoords(int first, int second) const {
  return first * kColumsCount_ + second +
         current_page * kRowCount_ * kColumsCount_;
}

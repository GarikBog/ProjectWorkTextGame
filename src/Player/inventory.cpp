#include "inventory.h"

#include "../Basic/game_state.h"

Inventory::Inventory()
    : item_in_hand_(nullptr), armor_slot_(nullptr), helmet_slot_(nullptr) {
  for (int i = 0; i < BELT_COUNT; ++i) belt_[i] = nullptr;
}

void Inventory::AddItemtoInventory(Item* item) {
  if (!item) return;
  if (FindItemPosInInventory(item) == -1) inventory_.push_back(item);
  GameState::GetGameState().UpdateUIInvSlots();
}

void Inventory::RemoveItemFromInventory(Item* item) {
  int pos = FindItemPosInInventory(item);

  if (pos != -1) inventory_.erase(inventory_.begin() + pos);
  GameState::GetGameState().UpdateUIInvSlots();
}

void Inventory::AddItemToHandSlot(HandItem* item) {
  if (item != nullptr &&
      (FindItemPosInInventory(item) == -1 || item->GetType() != kHandItem))
    return;

  if (item_in_hand_) item_in_hand_->DeactiveItem();
  AddItemtoInventory(item_in_hand_);
  item_in_hand_ = item;
  RemoveItemFromInventory(item);

  if (item_in_hand_) item_in_hand_->ActiveItem();
}

void Inventory::AddItemToArmorSlot(DefensiveItem* item) {
  if ((item != nullptr &&
       (FindItemPosInInventory(item) == -1 || item->GetType() != kArmor)))
    return;

  if (armor_slot_) armor_slot_->DeactiveItem();
  AddItemtoInventory(armor_slot_);
  armor_slot_ = item;
  RemoveItemFromInventory(item);
  if (armor_slot_) armor_slot_->ActiveItem();
}

void Inventory::AddItemToHelmetSlot(DefensiveItem* item) {
  if ((item != nullptr &&
       (FindItemPosInInventory(item) == -1 || item->GetType() != kHelmet)))
    return;

  if (helmet_slot_) helmet_slot_->DeactiveItem();
  AddItemtoInventory(helmet_slot_);
  helmet_slot_ = item;
  RemoveItemFromInventory(item);
  if (helmet_slot_) helmet_slot_->ActiveItem();
}

void Inventory::AddItemToBelt(Item* item, int belt_slot) {
  if (belt_slot < 0 || belt_slot >= BELT_COUNT ||
      (item != nullptr &&
       (FindItemPosInInventory(item) == -1 || item->GetType() != kBeltItem)))
    return;

  if (belt_[belt_slot]) belt_[belt_slot]->DeactiveItem();
  AddItemtoInventory(belt_[belt_slot]);
  belt_[belt_slot] = item;
  RemoveItemFromInventory(item);
  if (belt_[belt_slot]) belt_[belt_slot]->ActiveItem();
}

void Inventory::AddItemToHandSlot(int item_pos) {
  if (item_pos >= inventory_.size()) return;
  HandItem* item = dynamic_cast<HandItem*>(inventory_[item_pos]);
  if (item) AddItemToHandSlot(item);
}

int Inventory::FindItemPosInInventory(Item* item) {
  for (int i = 0; i < inventory_.size(); ++i) {
    if (inventory_[i] == item) return i;
  }
  return -1;
}
void Inventory::AddItemToArmorSlot(int item_pos) {
  if (item_pos >= inventory_.size()) return;
  DefensiveItem* item = dynamic_cast<DefensiveItem*>(inventory_[item_pos]);
  if (item) AddItemToArmorSlot(item);
}

void Inventory::AddItemToHelmetSlot(int item_pos) {
  if (item_pos >= inventory_.size()) return;
  DefensiveItem* item = dynamic_cast<DefensiveItem*>(inventory_[item_pos]);
  if (item) AddItemToHelmetSlot(item);
}

void Inventory::AddItemToBelt(int item_pos, int belt_slot) {
  if (item_pos >= inventory_.size()) return;
  AddItemToBelt(inventory_[item_pos], belt_slot);
}

int Inventory::FindItemPosInBelt(Item* item) {
  for (int i = 0; i < BELT_COUNT; ++i) {
    if (belt_[i] == item) return i;
  }
  return -1;
}

bool Inventory::DoesHasItem(Item* item) {
  return (item_in_hand_ == item || armor_slot_ == item ||
          helmet_slot_ == item || (FindItemPosInBelt(item) != -1) ||
          (FindItemPosInInventory(item) != -1));
}

bool Inventory::DoesHasItem(std::string name) {
  if (item_in_hand_ && item_in_hand_->GetName() == name) return true;
  if (armor_slot_ && armor_slot_->GetName() == name) return true;
  if (helmet_slot_ && helmet_slot_->GetName() == name) return true;

  for (int i = 0; i < BELT_COUNT; ++i)
    if (belt_[i]->GetName() == name) return true;
  for (int i = 0; i < inventory_.size(); ++i)
    if (inventory_[i]->GetName() == name) return true;

  return false;
}

int Inventory::GetItemCount() const { return inventory_.size(); }

Icon* Inventory::GetItemIcon(int item_pos) {
  if (item_pos >= inventory_.size())
    throw std::exception("Try to get Item in pos > items count!\n");
  return &inventory_[item_pos]->GetIcon();
}

Icon* Inventory::GetHandItemIcon() {
  if (item_in_hand_) return &item_in_hand_->GetIcon();
  return nullptr;
}
Icon* Inventory::GetArmorItemIcon() {
  if (armor_slot_) return &armor_slot_->GetIcon();
  return nullptr;
}

Icon* Inventory::GetHelmetItemIcon() {
  if (helmet_slot_) return &helmet_slot_->GetIcon();
  return nullptr;
}

Icon* Inventory::GetBeltItemIcon(int slot) {
  if (slot >= BELT_COUNT)
    throw std::exception("Try to get belt item icon with slot >= BELT_CONT!\n");
  if (belt_[slot]) return &belt_[slot]->GetIcon();
  return nullptr;
}

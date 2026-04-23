#ifndef INVENTORY
#define INVENTORY

#define BELT_COUNT 5
#include <vector>

#include "../Items/item.h"

class Inventory {
 public:
  Inventory();

  void AddItemtoInventory(Item* item);
  void RemoveItemFromInventory(Item* item);

  void AddItemToHandSlot(HandItem* item);

  void AddItemToArmorSlot(DefensiveItem* item);
  void AddItemToHelmetSlot(DefensiveItem* item);

  void AddItemToBelt(Item* item, int belt_slot);

  void AddItemToHandSlot(int item_pos);

  void AddItemToArmorSlot(int item_pos);
  void AddItemToHelmetSlot(int item_pos);

  void AddItemToBelt(int item_pos, int belt_slot);

  int FindItemPosInInventory(Item* item);
  int FindItemPosInBelt(Item* item);

  bool DoesHasItem(Item* item);
  bool DoesHasItem(std::string name);

  int GetItemCount() const;

  Icon* GetItemIcon(int item_pos);
  Icon* GetHandItemIcon();
  Icon* GetArmorItemIcon();
  Icon* GetHelmetItemIcon();
  Icon* GetBeltItemIcon(int slot);

 private:
  HandItem* item_in_hand_;
  DefensiveItem* armor_slot_;
  DefensiveItem* helmet_slot_;
  Item* belt_[BELT_COUNT];

  std::vector<Item*> inventory_;
};
#endif  // !INVENTORY

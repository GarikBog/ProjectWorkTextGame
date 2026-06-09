#ifndef ITEM_FABRIC
#define ITEM_FABRIC
#include "item.h"

class ItemFabric {
 public:
  static Item* GetItem(Items item_name);

 private:
  static inline std::map<std::string, std::string> item_paths = {
      {"test_armor", "/items/TestArmor.txt"},
      {"test_item", "/items/TestItem.txt"},
      {"amulet", "/items/amulet.txt"},
      {"stick", "/items/stick.txt"},
      {"sword", "/items/Sword.txt"}};
};

#endif  //

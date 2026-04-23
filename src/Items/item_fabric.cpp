#include "item_fabric.h"

Item* ItemFabric::GetItem(Items item_name) {
  switch (item_name) {
    case kDefaultArmor:
      return new DefensiveItem(ASSETS_PATH "/items/TestArmor.txt");
    case kMaskOfMadness:
      return new DefensiveItem(ASSETS_PATH "/items/TestItem.txt", kHelmet);
    case kAmulet:
      return new StatItem(ASSETS_PATH "/items/amulet.txt");
    case kMagicStick:
      return new HandItem(ASSETS_PATH "/items/stick.txt");
    case kSword:
      return new HandItem(ASSETS_PATH "/items/Sword.txt");
    default:
      return new DefensiveItem(ASSETS_PATH "/items/TestArmor.txt");
  }
}

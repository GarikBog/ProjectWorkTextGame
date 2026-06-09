#include "item_fabric.h"

Item* ItemFabric::GetItem(Items item_name) {
  switch (item_name) {
    case kDefaultArmor:
      return new DefensiveItem(ASSETS_PATH + item_paths.at("test_armor"));
    case kMaskOfMadness:
      return new DefensiveItem(ASSETS_PATH + item_paths.at("test_item"),
                               kHelmet);
    case kAmulet:
      return new StatItem(ASSETS_PATH + item_paths.at("amulet"));
    case kMagicStick:
      return new HandItem(ASSETS_PATH + item_paths.at("stick"));
    case kSword:
      return new HandItem(ASSETS_PATH + item_paths.at("sword"));
    default:
      return new DefensiveItem(ASSETS_PATH + item_paths.at("test_armor"));
  }
}

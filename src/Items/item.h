#ifndef ITEM
#define ITEM

#include <string>

#include "../Basic/stats.h"
#include "../Visual/icon.h"

enum ItemTypes { kDefaultItem, kArmor, kHelmet, kBeltItem, kHandItem };
enum Items {
  // armour
  kDefaultArmor,

  // helmets
  kMaskOfMadness,

  // stat belt
  kAmulet,

  // hand
  kMagicStick,
  kSword

};

class Item {
 public:
  Item(std::string asset_file_path);
  virtual ~Item();

  virtual void ActiveItem();
  virtual void DeactiveItem();

  ItemTypes GetType() const;
  std::string GetName() const;
  Icon& GetIcon();

 protected:
  std::string name_;
  Icon icon_;
  ItemTypes type_;
};

class DefensiveItem : public Item {
 public:
  DefensiveItem(std::string asset_file_path, ItemTypes type = kArmor);
  virtual ~DefensiveItem() override;

  virtual void ActiveItem() override;
  virtual void DeactiveItem() override;

 protected:
  float defensive_power_;
};

class HandItem : public Item {
 public:
  HandItem(std::string asset_file_path);

  virtual void ActiveItem() override;
  virtual void DeactiveItem() override;

 private:
  float damage_;
};

class BeltItem : public Item {
 public:
  BeltItem(std::string asset_file_path);

  virtual void ActiveItem() = 0;
  virtual void DeactiveItem() = 0;
};

class StatItem : public BeltItem {
 public:
  StatItem(std::string asset_file_path);

  virtual void ActiveItem() override;
  virtual void DeactiveItem() override;

 private:
  BattleStats battle_stats_diff_;
  Stats stats_diff_;
};

#endif  //

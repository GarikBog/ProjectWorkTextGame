#ifndef LOOTABLE_OBJECT
#define LOOTABLE_OBJECT

#include "../Items/item.h"
#include "object.h"

class LootableObject : public Object {
 public:
  virtual void Check() override;

  std::string GetName() const;

  LootableObject(std::string texture_file, sf::IntRect texture_rect,
                 std::string asset_file_path, std::string name = "",
                 BattleStats& battle_stats = BattleStats());

  virtual ~LootableObject();

 protected:
  std::vector<Item*> items_in_;
};

#endif  // !LO

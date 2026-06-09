#ifndef PLAYER
#define PLAYER

#include "../Basic/stats.h"
#include "../FightSystem/player_character.h"
#include "inventory.h"

class Player {
 public:
  void SetPlayerCharacter(PlayerCharacter* player_character);

  Player();
  BattleStats& GetBattleStats();
  PlayerCharacter* GetPlayerCharacter();
  Inventory& GetInventory();

  void ChangeDefensive(float count);
  void ChangeDamage(int count);
  bool IsAlive() const;

  void Kill();

  float GetStatByString(std::string stat_name);

 private:
  Inventory inventory_;
  BattleStats battle_stats_;
  Stats stats_;
  PlayerCharacter* player_character_;
  bool is_alive;
};
#endif  // !PLAYER

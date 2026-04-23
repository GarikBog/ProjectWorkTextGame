#ifndef STATS
#define STATS

#ifndef IOSTREAM
#include <iostream>
#define IOSTREAM
#endif

struct BattleStats {
  BattleStats(int max_hp = 0, int stamina = 0, int min_attack_range = 0,
              int max_attack_range = 0, int damage = 5, float defensive = 0,
              int max_attack_count = 1);

  bool GetDamage(int damage);
  int CalcDamageForMe(int damage);

  int stamina;
  int current_stamina;
  int health;
  int max_health;
  int min_attack_range;
  int max_attack_range;
  int damage;
  float defensive;
  int attack_count;
  int max_attack_count;

  std::string GetStatsInString();
  void NewTurn();

  BattleStats& operator=(BattleStats& other);
  BattleStats& operator+=(BattleStats& other);
  BattleStats& operator-=(BattleStats& other);

 private:
  int image_min_attack_range;
};

struct Stats {
  std::string GetStatsInString();
};

#endif  //

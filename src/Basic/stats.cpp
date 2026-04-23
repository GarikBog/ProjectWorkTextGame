#include "stats.h"

#include <string>

BattleStats::BattleStats(int max_hp, int stamina, int min_attack_range,
                         int max_attack_range, int damage, float defensive,
                         int max_attack_count)
    : max_health(max_hp),
      health(max_hp),
      stamina(stamina),
      current_stamina(stamina),
      min_attack_range(min_attack_range),
      max_attack_range(max_attack_range),
      defensive(defensive),
      damage(damage),
      image_min_attack_range(min_attack_range),
      max_attack_count(max_attack_count),
      attack_count(max_attack_count) {}

bool BattleStats::GetDamage(int damage) {
  health = std::max(0, health - damage);
  return health > 0;
}

int BattleStats::CalcDamageForMe(int damage) {
  damage *= std::max(0.f, 1 - defensive);
  return health - damage < 0 ? health : damage;
}

std::string BattleStats::GetStatsInString() {
  return "Health: " + std::to_string(health) + " / " +
         std::to_string(max_health) + "\nDamage: " + std::to_string(damage) +
         "\nDefensive: " + std::to_string(int(defensive * 100)) + "%" +
         "\nStamina: " + std::to_string(current_stamina) + " / " +
         std::to_string(stamina) +
         "\nAttack Range: " + std::to_string(min_attack_range) + "-" +
         std::to_string(max_attack_range) +
         "\nCount of attacks: " + std::to_string(attack_count) + "/" +
         std::to_string(max_attack_count);
}

void BattleStats::NewTurn() {
  current_stamina = stamina;
  attack_count = max_attack_count;
}

BattleStats& BattleStats::operator=(BattleStats& other) {
  this->max_health = other.max_health;
  this->health = other.health;
  this->stamina = other.stamina;
  this->current_stamina = other.current_stamina;
  this->min_attack_range = other.min_attack_range;
  this->max_attack_range = other.max_attack_range;
  this->defensive = other.defensive;
  this->damage = other.damage;
  this->image_min_attack_range = other.min_attack_range;
  this->attack_count = other.attack_count;
  this->max_attack_count = other.max_attack_count;
  return *this;
}

BattleStats& BattleStats::operator+=(BattleStats& other) {
  this->max_health += other.max_health;
  this->health += other.health;
  this->stamina += other.stamina;
  this->current_stamina += other.current_stamina;
  this->image_min_attack_range += other.min_attack_range;
  if (this->min_attack_range + other.min_attack_range >= 0)
    this->min_attack_range = this->image_min_attack_range;
  else
    this->min_attack_range = 0;
  this->max_attack_range += other.max_attack_range;
  this->defensive += other.defensive;
  this->damage += other.damage;
  this->attack_count += other.attack_count;
  this->max_attack_count += other.max_attack_count;
  return *this;
}

BattleStats& BattleStats::operator-=(BattleStats& other) {
  this->max_health -= other.max_health;
  this->health -= other.health;
  this->stamina -= other.stamina;
  this->current_stamina -= other.current_stamina;
  this->image_min_attack_range += other.min_attack_range;
  if (this->min_attack_range - other.min_attack_range >= 0)
    this->min_attack_range = this->image_min_attack_range;
  else
    this->min_attack_range = 0;
  this->max_attack_range -= other.max_attack_range;
  this->defensive -= other.defensive;
  this->damage -= other.damage;
  this->attack_count -= other.attack_count;
  this->max_attack_count -= other.max_attack_count;
  return *this;
}

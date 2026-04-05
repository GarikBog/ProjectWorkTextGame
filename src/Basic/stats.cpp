#include "stats.h"

#include <string>
std::string Stats::GetStatsInString() {
  return "\nMaxHealth: " + std::to_string(max_health) +
         "\nHealth: " + std::to_string(health) +
         "\nStamina: " + std::to_string(health) +
         "\nCurrentStamina: " + std::to_string(max_health);
}

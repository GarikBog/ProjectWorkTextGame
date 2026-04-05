#ifndef STATS
#define STATS

#ifndef IOSTREAM
#include <iostream>
#define IOSTREAM
#endif

struct Stats {
  int stamina;
  int current_stamina;
  int health;
  int max_health;

  std::string GetStatsInString();
};

#endif  //

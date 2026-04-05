#ifndef PLAYER
#define PLAYER

#include "../Basic/stats.h"

class Player {
 public:
  Player();
  Stats& GetStats() { return stats; }

 private:
  Stats stats;
};
#endif  // !PLAYER

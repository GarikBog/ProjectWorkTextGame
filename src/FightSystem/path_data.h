#ifndef PATH_DATA_H_
#define PATH_DATA_H_

#include <vector>

struct PathNode {
  int x;
  int y;
  float cost;

  PathNode(int x_, int y_, float cost_ = 0);
};

struct PathResult {
  std::vector<PathNode> path;
  float remaining_movement;
  float needed_movement;
  bool can_reach;

  PathResult();
};

#endif  // PATH_DATA_H_

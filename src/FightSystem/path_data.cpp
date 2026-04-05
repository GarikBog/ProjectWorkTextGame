#include "path_data.h"

PathNode::PathNode(int x_, int y_, float cost_) : x(x_), y(y_), cost(cost_) {}

PathResult::PathResult()
    : remaining_movement(0), needed_movement(0), can_reach(false) {}

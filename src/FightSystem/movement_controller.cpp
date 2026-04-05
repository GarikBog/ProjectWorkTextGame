#include "movement_controller.h"

#include <algorithm>
#include <cmath>
#include <queue>

#include "../Basic/game_state.h"
#include "battle_field.h"
#include "path_data.h"

namespace {
struct AStarNode {
  int x;
  int y;
  float g;
  float h;
  AStarNode* parent;

  AStarNode(int x_, int y_)
      : x(x_), y(y_), g(INFINITY), h(0), parent(nullptr) {}

  float f() const { return g + h; }
};

struct CompareNode {
  bool operator()(const AStarNode* a, const AStarNode* b) const {
    return a->f() > b->f();
  }
};

const int kDx[4] = {0, 1, 0, -1};
const int kDy[4] = {1, 0, -1, 0};
const float kTimePerCell = 0.3f;

}  // namespace

MovementController::MovementController(BattleField* battle_field)
    : battle_field_(battle_field),
      state_(MovementState::kIdle),
      current_character_(nullptr),
      start_x_(-1),
      start_y_(-1),
      target_x_(-1),
      target_y_(-1),
      total_speed_(0),
      current_step_index_(0),
      step_timer_(0) {}

MovementController::~MovementController() {}

float MovementController::GetCellCost(int x, int y) {
  Object* obj = battle_field_->GetCellObject(x, y);
  if (obj == nullptr) {
    return 1.0f;
  }
  return obj->GetMovementCost();
}

PathResult MovementController::CalculatePath(int start_x, int start_y,
                                             int target_x, int target_y,
                                             float speed) {
  PathResult result;

  int width = battle_field_->GetWidth();
  int height = battle_field_->GetHeight();

  if (start_x < 0 || start_x >= width || start_y < 0 || start_y >= height ||
      target_x < 0 || target_x >= width || target_y < 0 || target_y >= height) {
    result.can_reach = false;
    return result;
  }

  std::vector<std::vector<AStarNode*>> nodes;
  nodes.resize(width);
  for (int i = 0; i < width; ++i) {
    nodes[i].resize(height);
    for (int j = 0; j < height; ++j) {
      nodes[i][j] = new AStarNode(i, j);
      nodes[i][j]->h = std::abs(i - target_x) + std::abs(j - target_y);
    }
  }

  std::priority_queue<AStarNode*, std::vector<AStarNode*>, CompareNode>
      open_set;
  nodes[start_x][start_y]->g = 0;
  open_set.push(nodes[start_x][start_y]);

  AStarNode* current = nullptr;

  while (!open_set.empty()) {
    current = open_set.top();
    open_set.pop();

    if (current->x == target_x && current->y == target_y) {
      break;
    }

    for (int i = 0; i < 4; ++i) {
      int nx = current->x + kDx[i];
      int ny = current->y + kDy[i];

      if (nx < 0 || nx >= width || ny < 0 || ny >= height) {
        continue;
      }

      float step_cost = GetCellCost(nx, ny);
      float new_g = current->g + step_cost;

      if (new_g < nodes[nx][ny]->g) {
        nodes[nx][ny]->g = new_g;
        nodes[nx][ny]->parent = current;
        open_set.push(nodes[nx][ny]);
      }
    }
  }

  if (current != nullptr && current->x == target_x && current->y == target_y) {
    std::vector<std::pair<int, int>> raw_path;
    AStarNode* node = current;
    while (node != nullptr) {
      raw_path.push_back({node->x, node->y});
      node = node->parent;
    }
    std::reverse(raw_path.begin(), raw_path.end());

    float total_cost = 0.0f;
    for (size_t i = 1; i < raw_path.size(); ++i) {
      float cell_cost = GetCellCost(raw_path[i].first, raw_path[i].second);
      total_cost += cell_cost;
      result.path.push_back(
          PathNode(raw_path[i].first, raw_path[i].second, cell_cost));
    }

    result.needed_movement = total_cost;
    result.remaining_movement = speed - total_cost;
    result.can_reach = true;

    result.path.insert(result.path.begin(),
                       PathNode(raw_path[0].first, raw_path[0].second, 0.0f));
  } else {
    result.can_reach = false;
    result.needed_movement = INFINITY;
  }

  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < height; ++j) {
      delete nodes[i][j];
    }
  }

  return result;
}

bool MovementController::TryMoveToCell(int target_x, int target_y,
                                       float speed) {
  if (current_character_ == nullptr) {
    return false;
  }

  int char_x, char_y;
  if (!battle_field_->GetCharacterPosition(current_character_, char_x,
                                           char_y)) {
    return false;
  }

  start_x_ = char_x;
  start_y_ = char_y;
  target_x_ = target_x;
  target_y_ = target_y;
  total_speed_ = speed;

  current_path_ =
      CalculatePath(start_x_, start_y_, target_x_, target_y_, speed);

  if (current_path_.can_reach) {
    HighlightPath(current_path_);
    state_ = MovementState::kPathPreview;
    return true;
  }
  return false;
}

void MovementController::ButtonIsPressed(Button* button) {
  BattleCell* cell = battle_field_->GetCellFromButton(button);
  if (cell == nullptr) {
    return;
  }

  int cell_x, cell_y;
  battle_field_->GetCellCoordinates(cell, cell_x, cell_y);

  switch (state_) {
    case MovementState::kIdle:
      if (cell->GetObjectOnCell() != nullptr) {
        ClearHighlights();
        current_character_ = cell->GetObjectOnCell();
        start_x_ = cell_x;
        start_y_ = cell_y;
        GameState::GetGameState().PrintText("Character selected",
                                            sf::Color::Blue);
        state_ = MovementState::kPathPreview;
      }
      break;

    case MovementState::kPathPreview:
      if (target_x_ == -1 && target_y_ == -1) {
        ClearHighlights();
        TryMoveToCell(cell_x, cell_y, 10.0f);
      } else if (cell_x == target_x_ && cell_y == target_y_) {
        StartMoving(current_path_);
        state_ = MovementState::kMoving;
        ClearHighlights();
      } else {
        ClearHighlights();
        TryMoveToCell(cell_x, cell_y, total_speed_);
      }
      break;

    case MovementState::kMoving:
      break;
  }
}

void MovementController::StartMoving(const PathResult& path) {
  current_step_index_ = 1;
  step_timer_ = 0.0f;
  GameState::GetGameState().PrintText("Moving...", sf::Color::White);
}

void MovementController::Update(float delta_time) {
  if (state_ != MovementState::kMoving) {
    return;
  }

  step_timer_ += delta_time;

  if (step_timer_ >= kTimePerCell) {
    step_timer_ = 0.0f;

    if (current_step_index_ >= static_cast<int>(current_path_.path.size())) {
      state_ = MovementState::kIdle;
      current_character_ = nullptr;
      GameState::GetGameState().PrintText("Movement complete!",
                                          sf::Color::Green);
      return;
    }

    const PathNode& to_node = current_path_.path[current_step_index_];
    BattleCell* to_cell = battle_field_->GetCell(to_node.x, to_node.y);

    int from_x = current_path_.path[current_step_index_ - 1].x;
    int from_y = current_path_.path[current_step_index_ - 1].y;
    BattleCell* from_cell = battle_field_->GetCell(from_x, from_y);
    if (to_cell != nullptr && to_cell->ObjectMovesToCell(current_character_) &&
        from_cell->ObjectMovesFromCell(current_character_)) {
      if (from_cell != nullptr) {
        from_cell->SetObjectOnCell(nullptr);
      }

      to_cell->SetObjectOnCell(current_character_);
      current_step_index_++;

      float cost_so_far = 0.0f;
      for (int i = 1; i < current_step_index_; ++i) {
        cost_so_far += current_path_.path[i].cost;
      }

      if (cost_so_far > total_speed_) {
        state_ = MovementState::kIdle;
        current_character_ = nullptr;
        GameState::GetGameState().PrintText("Out of speed! Stopped.",
                                            sf::Color::Yellow);
      }
    } else {
      state_ = MovementState::kIdle;
      current_character_ = nullptr;
      GameState::GetGameState().PrintText("Path blocked! Stopped.",
                                          sf::Color::Red);
    }
  }
}

void MovementController::HighlightPath(const PathResult& path) {
  for (size_t i = 1; i < path.path.size(); ++i) {
    float cost_so_far = 0.0f;
    for (size_t j = 1; j <= i; ++j) {
      cost_so_far += path.path[j].cost;
    }

    sf::Color color =
        (cost_so_far <= total_speed_) ? sf::Color::Green : sf::Color::Red;
    battle_field_->HighlightCell(path.path[i].x, path.path[i].y, color);
  }
}

void MovementController::ClearHighlights() {
  battle_field_->ClearAllHighlights();
}

MovementController::MovementState MovementController::GetState() const {
  return state_;
}

void MovementController::SetCurrentCharacter(Object* character) {
  current_character_ = character;
}

Object* MovementController::GetCurrentCharacter() const {
  return current_character_;
}

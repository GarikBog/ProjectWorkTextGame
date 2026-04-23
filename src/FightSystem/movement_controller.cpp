#include "movement_controller.h"

#include <algorithm>
#include <cmath>
#include <queue>

#include "../Basic/game_state.h"
#include "../Visual/character_info_widget.h"
#include "battle_field.h"
#include "path_data.h"
#include "player_character.h"

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
      current_step_index_(0),
      step_timer_(0),
      info_widget_(nullptr),
      is_player_turn_(true),
      current_npc_index_(0),
      is_npc_moving_(false),
      current_moving_npc_(nullptr) {
  FindAllNPCs();

  if (GameState::GetGameState().GetCurrentBattle()) {
    SetInfoWidget(
        GameState::GetGameState().GetCurrentBattle()->GetInfoWidget());
  }
}
MovementController::~MovementController() {}

void MovementController::FindAllNPCs() {
  npc_characters_.clear();
  for (int i = 0; i < battle_field_->GetWidth(); ++i) {
    for (int j = 0; j < battle_field_->GetHeight(); ++j) {
      Object* obj = battle_field_->GetCellObject(i, j);
      if (obj != nullptr && obj->GetType() == kEnemyCharacter) {
        Character* character = dynamic_cast<Character*>(obj);
        if (character != nullptr) {
          npc_characters_.push_back(character);
          std::cout << "Found NPC at: " << i << "," << j << std::endl;
        }
      }
    }
  }
  std::cout << "Total NPCs found: " << npc_characters_.size() << std::endl;

  if (npc_characters_.empty()) battle_field_->EndBattle();
}

float MovementController::GetCellCost(int x, int y) {
  Object* obj = battle_field_->GetCellObject(x, y);

  // Если клетка пустая - проходима со стоимостью 1
  if (obj == nullptr) {
    return 1.0f;
  }

  // Клетка с игроком - проходима
  if (obj->GetType() == kPlayerCharacter) {
    return 1.0f;
  }

  // Клетка с текущим двигающимся NPC - проходима (сам NPC)
  if (current_moving_npc_ != nullptr && obj == current_moving_npc_) {
    return 1.0f;
  }

  // Клетка с другим NPC - непроходима
  if (obj->GetType() == kEnemyCharacter) {
    return INFINITY;
  }

  // Если на клетке есть объект, который нельзя проходить
  if (!obj->IsWalkable()) {
    return INFINITY;
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
  if (!is_player_turn_) return;
  BattleCell* cell = battle_field_->GetCellFromButton(button);
  if (cell == nullptr) {
    return;
  }

  int cell_x, cell_y;
  battle_field_->GetCellCoordinates(cell, cell_x, cell_y);
  Object* clicked_object = cell->GetObjectOnCell();

  // Для ЛЮБОГО персонажа (игрок, враг) показываем информацию в виджете
  if (clicked_object != nullptr && info_widget_ != nullptr) {
    info_widget_->SetObject(clicked_object);
  }
  if (clicked_object != nullptr) {
    state_ = MovementState::kIdle;
  }
  switch (state_) {
    case MovementState::kIdle: {
      // Если это игровой персонаж - даём управление
      if (clicked_object == nullptr) break;
      // Если это тот же персонаж, который уже выбран - снимаем выделение
      if (current_character_ == clicked_object) {
        SetCurrentCharacter(nullptr);
        state_ = MovementState::kIdle;
      } else {
        SetCurrentCharacter(clicked_object);
        state_ = MovementState::kIdle;
        if (clicked_object->GetType() == kPlayerCharacter) {
          // Выбираем нового персонажа ДЛЯ УПРАВЛЕНИЯ
          battle_field_->GetCharacterPosition(current_character_, start_x_,
                                              start_y_);
          target_x_ = -1;
          target_y_ = -1;
          state_ = MovementState::kPathPreview;
        }
      }
    } break;

    case MovementState::kPathPreview:
      // Если кликнули на клетку с выбранным персонажем - снимаем выделение
      if (clicked_object == current_character_) {
        ClearHighlights();
        SetCurrentCharacter(nullptr);
        target_x_ = -1;
        target_y_ = -1;
        if (info_widget_ != nullptr) {
          info_widget_->ClearCharacter();
        }
        state_ = MovementState::kIdle;
        break;
      }

      if (target_x_ == -1 && target_y_ == -1) {
        ClearHighlights();
        if (current_character_ != nullptr) {
          Character* character = static_cast<Character*>(current_character_);
          float current_stamina =
              static_cast<float>(character->GetStats().current_stamina);
          TryMoveToCell(cell_x, cell_y, current_stamina);
        }
      } else if (cell_x == target_x_ && cell_y == target_y_) {
        StartMoving(current_path_);
        state_ = MovementState::kMoving;
        ClearHighlights();
      } else {
        ClearHighlights();
        if (current_character_ != nullptr) {
          Character* character = static_cast<Character*>(current_character_);
          float current_stamina =
              static_cast<float>(character->GetStats().current_stamina);
          TryMoveToCell(cell_x, cell_y, current_stamina);
        }
      }
      break;

    case MovementState::kMoving:
      break;
  }
}

void MovementController::StartMoving(const PathResult& path) {
  current_step_index_ = 1;
  step_timer_ = 0.0f;
}
void MovementController::Update(float delta_time) {
  if (is_npc_moving_ && current_moving_npc_ != nullptr) {
    step_timer_ += delta_time;
    if (step_timer_ >= kTimePerCell) {
      step_timer_ = 0.0f;
      if (current_step_index_ >=
          static_cast<int>(npc_current_path_.path.size())) {
        int stamina_used = static_cast<int>(npc_current_path_.needed_movement);
        current_moving_npc_->GetStats().current_stamina -= stamina_used;
        if (current_moving_npc_->GetStats().current_stamina < 0) {
          current_moving_npc_->GetStats().current_stamina = 0;
        }
        is_npc_moving_ = false;
        current_npc_index_++;
        StartNextNPC();
        return;
      }
      const PathNode& to_node = npc_current_path_.path[current_step_index_];
      BattleCell* to_cell = battle_field_->GetCell(to_node.x, to_node.y);
      int from_x = npc_current_path_.path[current_step_index_ - 1].x;
      int from_y = npc_current_path_.path[current_step_index_ - 1].y;
      BattleCell* from_cell = battle_field_->GetCell(from_x, from_y);
      if (to_cell != nullptr &&
          to_cell->ObjectMovesToCell(current_moving_npc_) &&
          from_cell->ObjectMovesFromCell(current_moving_npc_)) {
        if (from_cell != nullptr) {
          from_cell->SetObjectOnCell(nullptr);
        }
        to_cell->SetObjectOnCell(current_moving_npc_);
        current_step_index_++;
      } else {
        is_npc_moving_ = false;
        current_npc_index_++;
        StartNextNPC();
      }
    }
    return;
  }
  if (state_ != MovementState::kMoving) {
    return;
  }

  step_timer_ += delta_time;

  if (step_timer_ >= kTimePerCell) {
    step_timer_ = 0.0f;

    if (current_step_index_ >= static_cast<int>(current_path_.path.size())) {
      // Движение завершено
      if (current_character_ != nullptr) {
        Character* character = static_cast<Character*>(current_character_);
        if (info_widget_ != nullptr &&
            character->GetType() == kPlayerCharacter) {
          info_widget_->UpdateText();
        }
      }
      state_ = MovementState::kIdle;
      SetCurrentCharacter(nullptr);
      return;
    }

    const PathNode& to_node = current_path_.path[current_step_index_];

    // Вычисляем стоимость следующего шага ДО того, как двигаться
    float step_cost = to_node.cost;

    Character* character = static_cast<Character*>(current_character_);
    float current_stamina =
        static_cast<float>(character->GetStats().current_stamina);

    // Проверяем, хватит ли stamina на следующий шаг
    if (step_cost > current_stamina) {
      // Не хватает stamina - останавливаемся, не делая шаг
      if (info_widget_ != nullptr && character->GetType() == kPlayerCharacter) {
        info_widget_->UpdateText();
      }

      state_ = MovementState::kIdle;
      SetCurrentCharacter(nullptr);
      return;
    }

    // Хватает stamina - делаем шаг
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

      // Тратим stamina
      character->GetStats().current_stamina -= static_cast<int>(step_cost);
      if (character->GetStats().current_stamina < 0) {
        character->GetStats().current_stamina = 0;
      }

      if (info_widget_ != nullptr && character->GetType() == kPlayerCharacter) {
        info_widget_->UpdateText();
      }

      current_step_index_++;

    } else {
      state_ = MovementState::kIdle;
      SetCurrentCharacter(nullptr);
    }
  }
}

void MovementController::HighlightPath(const PathResult& path) {
  if (current_character_ == nullptr) return;

  Character* character = static_cast<Character*>(current_character_);
  float current_stamina =
      static_cast<float>(character->GetStats().current_stamina);

  for (size_t i = 1; i < path.path.size(); ++i) {
    float cost_so_far = 0.0f;
    for (size_t j = 1; j <= i; ++j) {
      cost_so_far += path.path[j].cost;
    }

    sf::Color color =
        (cost_so_far <= current_stamina) ? sf::Color::Green : sf::Color::Red;
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
  GameState::GetGameState().GetCurrentBattle()->HideAttackMenu();
  if (!character) {
    target_x_ = -1;
    target_y_ = -1;
    ClearHighlights();
    if (info_widget_ != nullptr) info_widget_->ClearCharacter();
  }
}

Object* MovementController::GetCurrentCharacter() const {
  return current_character_;
}

void MovementController::SetInfoWidget(CharacterInfoWidget* widget) {
  info_widget_ = widget;
}

void MovementController::EndTurn() {
  if (!is_player_turn_) return;

  is_player_turn_ = false;
  ClearHighlights();
  SetCurrentCharacter(nullptr);
  state_ = MovementState::kIdle;

  current_npc_index_ = 0;
  is_npc_moving_ = false;
  StartNextNPC();
}

void MovementController::RefreshTurn() {
  state_ = MovementState::kIdle;
  SetCurrentCharacter(nullptr);
  ClearHighlights();

  for (int i = 0; i < battle_field_->GetWidth(); ++i) {
    for (int j = 0; j < battle_field_->GetHeight(); ++j) {
      Object* obj = battle_field_->GetCellObject(i, j);
      if (obj != nullptr) obj->GetStats().NewTurn();
    }
  }

  FindAllNPCs();
  current_npc_index_ = 0;
  is_npc_moving_ = false;
  current_moving_npc_ = nullptr;

  is_player_turn_ = true;

  if (info_widget_ != nullptr) {
    info_widget_->UpdateText();
  }

  if (!battle_field_->IsBattleEnded())
    GameState::GetGameState().GetConsole().Log(
        "================Новый ход================");
}

void MovementController::StartNextNPC() {
  std::cout << "=== StartNextNPC ===" << std::endl;
  std::cout << "current_npc_index: " << current_npc_index_ << std::endl;
  std::cout << "npc_characters_.size(): " << npc_characters_.size()
            << std::endl;

  if (current_npc_index_ >= static_cast<int>(npc_characters_.size())) {
    std::cout << "All NPCs done, turn ends" << std::endl;
    is_player_turn_ = true;
    is_npc_moving_ = false;
    current_moving_npc_ = nullptr;
    RefreshTurn();
    return;
  }

  current_moving_npc_ = npc_characters_[current_npc_index_];
  std::cout << "Current NPC stamina: "
            << current_moving_npc_->GetStats().current_stamina << std::endl;

  if (current_moving_npc_->GetStats().current_stamina <= 0) {
    std::cout << "NPC has no stamina, skipping" << std::endl;
    current_npc_index_++;

    auto player_character =
        GameState::GetGameState().GetPlayer().GetPlayerCharacter();
    battle_field_->TryAttackCharacter(current_moving_npc_, player_character,
                                      player_character->GetRandomBodyPart());

    StartNextNPC();
    return;
  }

  int npc_x, npc_y;
  if (!battle_field_->GetCharacterPosition(current_moving_npc_, npc_x, npc_y)) {
    std::cout << "Cannot find NPC position, skipping" << std::endl;
    current_npc_index_++;

    auto player_character =
        GameState::GetGameState().GetPlayer().GetPlayerCharacter();

    StartNextNPC();
    return;
  }
  std::cout << "NPC position: " << npc_x << "," << npc_y << std::endl;

  int player_x = -1, player_y = -1;
  battle_field_->GetCharacterPosition(
      GameState::GetGameState().GetPlayer().GetPlayerCharacter(), player_x,
      player_y);

  if (player_x == -1) {
    std::cout << "Cannot find player, skipping" << std::endl;
    current_npc_index_++;
    StartNextNPC();
    return;
  }
  std::cout << "Player position: " << player_x << "," << player_y << std::endl;

  int distance =
      battle_field_
          ->GetLineBetweenTwoObjects(
              current_moving_npc_,
              GameState::GetGameState().GetPlayer().GetPlayerCharacter())
          .size();
  int min_range = current_moving_npc_->GetStats().min_attack_range;
  int max_range = current_moving_npc_->GetStats().max_attack_range;
  std::cout << "Distance: " << distance << ", min_range: " << min_range
            << ", max_range: " << max_range << std::endl;

  // Если уже в зоне атаки - не двигаемся
  if (distance >= min_range && distance <= max_range) {
    std::cout << "Already in attack range, skipping" << std::endl;
    current_npc_index_++;

    auto player_character =
        GameState::GetGameState().GetPlayer().GetPlayerCharacter();
    battle_field_->TryAttackCharacter(current_moving_npc_, player_character,
                                      player_character->GetRandomBodyPart());

    StartNextNPC();
    return;
  }

  // Рассчитываем путь до игрока
  std::cout << "Calculating path to player..." << std::endl;
  npc_current_path_ =
      CalculatePath(npc_x, npc_y, player_x, player_y,
                    current_moving_npc_->GetStats().current_stamina);

  std::cout << "Path can_reach: " << npc_current_path_.can_reach << std::endl;
  std::cout << "Path size: " << npc_current_path_.path.size() << std::endl;

  if (npc_current_path_.can_reach && npc_current_path_.path.size() > 1) {
    float stamina_used = 0;
    int target_step = 1;

    for (size_t i = 1; i < npc_current_path_.path.size(); ++i) {
      int step_x = npc_current_path_.path[i].x;
      int step_y = npc_current_path_.path[i].y;
      int step_distance =
          std::abs(step_x - player_x) + std::abs(step_y - player_y);

      std::cout << "Step " << i << ": (" << step_x << "," << step_y
                << ") distance to player: " << step_distance << std::endl;

      if (step_distance < min_range) {
        std::cout << "Would be too close, stopping before this step"
                  << std::endl;
        break;
      }

      if (stamina_used + npc_current_path_.path[i].cost >
          current_moving_npc_->GetStats().current_stamina) {
        std::cout << "Not enough stamina for this step" << std::endl;
        break;
      }

      stamina_used += npc_current_path_.path[i].cost;
      target_step = i;
      std::cout << "Target step updated to: " << target_step
                << ", stamina used: " << stamina_used << std::endl;
    }

    std::cout << "Final target_step: " << target_step << std::endl;

    if (target_step > 0) {
      PathResult trimmed_path;
      trimmed_path.can_reach = true;
      trimmed_path.needed_movement = stamina_used;
      for (int i = 0; i <= target_step; ++i) {
        trimmed_path.path.push_back(npc_current_path_.path[i]);
      }
      npc_current_path_ = trimmed_path;

      std::cout << "Starting NPC movement, path size: "
                << npc_current_path_.path.size() << std::endl;
      is_npc_moving_ = true;
      current_step_index_ = 1;
      step_timer_ = 0.0f;

      return;
    } else {
      std::cout << "target_step is 0, cannot move" << std::endl;
    }
  } else {
    std::cout << "Path invalid or too short" << std::endl;
  }

  std::cout << "Cannot move, skipping to next NPC" << std::endl;
  current_npc_index_++;
  StartNextNPC();
}

void MovementController::UnregisterObject(Object* object) {
  if (object == nullptr) return;

  if (current_character_ == object) {
    current_character_ = nullptr;
    if (info_widget_ != nullptr) {
      info_widget_->ClearCharacter();
    }
    ClearHighlights();
    state_ = MovementState::kIdle;
  }

  if (current_moving_npc_ == object) {
    current_moving_npc_ = nullptr;
    is_npc_moving_ = false;
  }

  auto it = std::find(npc_characters_.begin(), npc_characters_.end(), object);
  if (it != npc_characters_.end()) {
    npc_characters_.erase(it);
    if (current_npc_index_ > 0 &&
        static_cast<size_t>(current_npc_index_) > npc_characters_.size()) {
      current_npc_index_ = npc_characters_.size();
    }
  }
}

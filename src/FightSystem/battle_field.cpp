#include "battle_field.h"

#include "../Basic/game_state.h"
#include "../Basic/object_fabric.h"
#include "battle_cell.h"

#ifndef FSTREAM
#define FSTREAM
#include <fstream>
#endif

#ifndef STRING
#include <string>
#define STRING
#endif

#ifndef IOSTREAM
#include <iostream>
#define IOSTREAM
#endif

#include "movement_controller.h"

BattleField::BattleField(sf::RenderWindow& window, std::string map_file_path,
                         const std::pair<float, float> pos,
                         const std::string texture_file,
                         const sf::IntRect texture_rectangle)
    : BattleField(window, map_file_path,
                  new Icon(texture_file, texture_rectangle), pos) {
  self_made_icon_ = true;
}

BattleField::BattleField(sf::RenderWindow& window, std::string map_file_path,
                         Icon* icon, const std::pair<float, float> pos)
    : Widget(
          window, icon, pos,
          {kBattleFieldRectangleSizeScale_, kBattleFieldRectangleSizeScale_}),
      character_info_widget_(
          window,
          std::pair(pos.first, pos.second + kBattleFieldRectangleSizeScale_ +
                                   kMarginBetweenFieldAndButtons),
          std::pair(kNextTurnButtonWidth, kNextTurnButtonHeight)),
      end_turn_button_(
          this, window,
          std::pair(pos.first + kNextTurnButtonWidth + 10,
                    pos.second + kBattleFieldRectangleSizeScale_ +
                        kMarginBetweenFieldAndButtons),
          std::pair(kNextTurnButtonWidth, kNextTurnButtonHeight),
          ASSETS_PATH "/textures/buttons/EndTurn.png",
          sf::IntRect(0, 0, kNextTurnButtonWidth, kNextTurnButtonHeight)),
      body_choise_menu_(
          window,
          std::pair(pos.first + 410,
                    pos.second + kBattleFieldRectangleSizeScale_ +
                        kMarginBetweenFieldAndButtons + kNextTurnButtonHeight -
                        kBodyPartWidgetHeight),
          std::pair(kBodyPartWidgetWidth, kBodyPartWidgetHeight),
          ASSETS_PATH "/textures/buttons/BodyMenu.png", {0, 0, 400, 600}),
      is_battle_ended_(false) {
  CalcBattleCellSize();
  CreateCells(map_file_path);
  if (GameState::GetGameState().GetMovementController()) {
    GameState::GetGameState().GetMovementController()->SetInfoWidget(
        &character_info_widget_);
  }
  HideAttackMenu();
}

BattleField::~BattleField() {}

void BattleField::AddObject(Object* object) {
  for (Object* current_object : objects_on_field_) {
    if (current_object == object) return;
  }
  objects_on_field_.push_back(object);
}

void BattleField::Update() {}

void BattleField::EndBattle() {
  GameState::GetGameState().GetConsole().Log(
      "==============Битва Окончена=============");
  is_battle_ended_ = true;
}

bool BattleField::IsBattleEnded() const { return is_battle_ended_; }

void BattleField::Draw() {
  Widget::Draw();
  for (int i = 0; i < kCountBattleCells_; ++i) {
    for (int j = 0; j < kCountBattleCells_; ++j) {
      if (cells_[i][j]) {
        cells_[i][j]->Draw();
      }
    }
  }
  character_info_widget_.Draw();
  end_turn_button_.Draw();
  body_choise_menu_.Draw();
}
void BattleField::SetVisibility(bool is_visible_now) {
  is_visible_ = is_visible_now;
  for (std::vector<BattleCell*> row : cells_)
    for (BattleCell* cell : row) {
      cell->SetVisibility(is_visible_now);
    }
  character_info_widget_.SetVisibility(is_visible_now);
  end_turn_button_.SetVisibility(is_visible_now);

  if (!is_visible_now) body_choise_menu_.SetVisibility(is_visible_now);
}
void BattleField::CreateCells(std::string map_file_path) {
  ClearCells();
  ClearObjects();

  auto CreateCell = [this](int x, int y, Object* basic_object,
                           Object* tmp_object) {
    float cell_pos_x =
        x_ + (kBorderBattleCellMargin_ +
              x * (battle_cell_size_ + kBetweenBattleCellMargin_));
    float cell_pos_y =
        y_ + (kBorderBattleCellMargin_ +
              y * (battle_cell_size_ + kBetweenBattleCellMargin_));

    cells_[x][y] = new BattleCell(
        *basic_object, tmp_object, window_, std::pair(cell_pos_x, cell_pos_y),
        std::pair(battle_cell_size_, battle_cell_size_));
    objects_on_field_.push_back(basic_object);
    if (tmp_object != nullptr) {
      objects_on_field_.push_back(tmp_object);
    }
  };

  std::ifstream file(map_file_path);

  if (!file.is_open()) {  // ���� �� �������� ����, ������� ������� ����
    throw std::exception("Cannot open battle file!");
  }

  cells_.resize(kCountBattleCells_);
  for (int i = 0; i < kCountBattleCells_; ++i) {
    cells_[i].resize(kCountBattleCells_);
    std::string line;

    if (!std::getline(file, line)) {
      file.close();
      return;
    }

    while (!line.empty() && (line.back() == '\r' || line.back() == '\n')) {
      line.pop_back();
    }

    for (int j = 0; j < kCountBattleCells_ && j < static_cast<int>(line.size());
         ++j) {
      char ch = line[j];
      Object* basic_object = ObjectFabric::GetObjectByChar(ch);
      CreateCell(i, j, basic_object, nullptr);
    }
  }

  int x, y;
  char symbol;
  while (file >> y >> x >> symbol) {
    if (x >= 0 && x < kCountBattleCells_ && y >= 0 && y < kCountBattleCells_) {
      Object* tmp_object = ObjectFabric::GetObjectByChar(symbol);
      if (tmp_object != nullptr && cells_[x][y] != nullptr) {
        cells_[x][y]->SetObjectOnCell(tmp_object);
        objects_on_field_.push_back(tmp_object);
      }
    }
  }

  file.close();
}

void BattleField::ClearCells() {
  for (std::vector<BattleCell*> row : cells_) {
    for (BattleCell* cell : row) {
      delete cell;
    }
  }
  cells_.clear();
}

void BattleField::ClearObjects() {
  for (Object* object : objects_on_field_) {
    delete object;
  }
  objects_on_field_.clear();
}

void BattleField::CalcBattleCellSize() {
  battle_cell_size_ =
      (kBattleFieldRectangleSizeScale_ - kBorderBattleCellMargin_ * 2 -
       ((kBetweenBattleCellMargin_)*kCountBattleCells_)) /
      kCountBattleCells_;
}

std::vector<std::pair<int, int>> BattleField::GetLineBetweenTwoCells(int x1,
                                                                     int y1,
                                                                     int x2,
                                                                     int y2) {
  std::vector<std::pair<int, int>> points;

  int dx = abs(x2 - x1);
  int dy = abs(y2 - y1);

  int sx = (x1 < x2) ? 1 : -1;
  int sy = (y1 < y2) ? 1 : -1;

  int err = dx - dy;

  int x = x1, y = y1;

  while (true) {
    points.push_back(std::pair(x, y));

    if (x == x2 && y == y2) {
      break;
    }

    int e2 = 2 * err;

    if (e2 > -dy) {
      err -= dy;
      x += sx;
    }
    if (e2 < dx) {
      err += dx;
      y += sy;
    }
  }

  return points;
}

std::vector<std::pair<int, int>> BattleField::GetLineBetweenTwoObjects(
    Object* first, Object* second) {
  int first_x, first_y, second_x, second_y;

  if (!GetCharacterPosition(first, first_x, first_y) ||
      !GetCharacterPosition(second, second_x, second_y))
    return std::vector<std::pair<int, int>>((-1, -1));

  std::vector<std::pair<int, int>> line_points =
      GetLineBetweenTwoCells(first_x, first_y, second_x, second_y);
  line_points.pop_back();
  return line_points;
}

void BattleField::ButtonIsPressed(Button* button) {
  if (button == &end_turn_button_) {
    GameState::GetGameState().GetMovementController()->EndTurn();
  }
}

Object* BattleField::GetCellObject(int x, int y) {
  if (x < 0 || x >= kCountBattleCells_ || y < 0 || y >= kCountBattleCells_) {
    return nullptr;
  }
  if (cells_[x][y] == nullptr) {
    return nullptr;
  }
  return cells_[x][y]->GetObjectOnCell();
}

bool BattleField::GetCharacterPosition(Object* character, int& out_x,
                                       int& out_y) {
  for (int i = 0; i < kCountBattleCells_; ++i) {
    for (int j = 0; j < kCountBattleCells_; ++j) {
      if (cells_[i][j] != nullptr &&
          cells_[i][j]->GetObjectOnCell() == character) {
        out_x = i;
        out_y = j;
        return true;
      }
    }
  }
  return false;
}

bool BattleField::PlaceObjectOnCell(Object* object, int x, int y) {
  if (x < 0 || x >= kCountBattleCells_ || y < 0 || y >= kCountBattleCells_) {
    return false;
  }
  if (cells_[x][y] == nullptr) {
    return false;
  }
  cells_[x][y]->SetObjectOnCell(object);
  return true;
}

bool BattleField::RemoveObjectFromCell(int x, int y) {
  if (x < 0 || x >= kCountBattleCells_ || y < 0 || y >= kCountBattleCells_) {
    return false;
  }
  if (cells_[x][y] == nullptr) {
    return false;
  }
  cells_[x][y]->SetObjectOnCell(nullptr);
  return true;
}

void BattleField::ObjectWasDestroyed(Object* object) {
  for (auto row : cells_)
    for (BattleCell* cell : row) {
      if (cell->GetObjectOnCell() == object) {
        cell->SetObjectOnCell(nullptr);
        break;
      }
    }
  for (int i = 0; i < objects_on_field_.size(); ++i) {
    if (objects_on_field_[i] == object) {
      objects_on_field_.erase(objects_on_field_.begin() + i);
    }
  }
  character_info_widget_.UnregisterObject(object);
}

void BattleField::ReplaceMe(Object* object, Object* replace_with) {
  for (auto row : cells_)
    for (BattleCell* cell : row) {
      if (cell->GetObjectOnCell() == object) {
        cell->SetObjectOnCell(replace_with);
        break;
      }
    }
}

void BattleField::HighlightCell(int x, int y, sf::Color color) {
  if (x < 0 || x >= kCountBattleCells_ || y < 0 || y >= kCountBattleCells_) {
    return;
  }
  if (cells_[x][y] != nullptr) {
    cells_[x][y]->SetHighlight(color);
  }
}

void BattleField::ClearAllHighlights() {
  for (int i = 0; i < kCountBattleCells_; ++i) {
    for (int j = 0; j < kCountBattleCells_; ++j) {
      if (cells_[i][j] != nullptr) {
        cells_[i][j]->ClearHighlight();
      }
    }
  }
}

void BattleField::GetCellCoordinates(BattleCell* cell, int& out_x, int& out_y) {
  for (int i = 0; i < kCountBattleCells_; ++i) {
    for (int j = 0; j < kCountBattleCells_; ++j) {
      if (cells_[i][j] == cell) {
        out_x = i;
        out_y = j;
        return;
      }
    }
  }
  out_x = -1;
  out_y = -1;
}

BattleCell* BattleField::GetCell(int x, int y) {
  if (x >= 0 && x < kCountBattleCells_ && y >= 0 && y < kCountBattleCells_) {
    return cells_[x][y];
  }
  return nullptr;
}

BattleCell* BattleField::GetCellFromButton(Button* button) {
  for (int i = 0; i < kCountBattleCells_; ++i) {
    for (int j = 0; j < kCountBattleCells_; ++j) {
      if (cells_[i][j] != nullptr && &cells_[i][j]->GetButton() == button) {
        return cells_[i][j];
      }
    }
  }
  return nullptr;
}

int BattleField::GetWidth() const { return kCountBattleCells_; }

int BattleField::GetHeight() const { return kCountBattleCells_; }

CharacterInfoWidget* BattleField::GetInfoWidget() {
  return &character_info_widget_;
}

void BattleField::SetMovementController(MovementController* controller) {
  for (int i = 0; i < kCountBattleCells_; ++i) {
    for (int j = 0; j < kCountBattleCells_; ++j) {
      if (cells_[i][j] != nullptr) {
        cells_[i][j]->SetMovementController(controller);
      }
    }
  }
}

void BattleField::ShowAttackMenu(Character* character) {
  body_choise_menu_.SetParts(character->GetBodyParts());
  body_choise_menu_.SetVisibility(true);
  end_turn_button_.SetVisibility(false);
}

void BattleField::HideAttackMenu() {
  body_choise_menu_.SetVisibility(false);
  end_turn_button_.SetVisibility(true);
}

void BattleField::TryAttackCharacter(Character* attacker, Character* attacked,
                                     BodyPart* in_part) {
  if (is_battle_ended_) return;
  auto line_points = GetLineBetweenTwoObjects(attacker, attacked);

  if (line_points.size() >= attacker->GetStats().min_attack_range &&
      line_points.size() <=
          attacker->GetStats().max_attack_range)  // attack range check
  {
    for (std::pair<int, int> point : line_points) {
      Object* on_cell = cells_[point.first][point.second]->GetObjectOnCell();
      if (on_cell && on_cell != attacker) {
        if (on_cell->IsAttackable()) {
          AttackObject(attacker, on_cell);
          return;
        } else {
          GameState::GetGameState().GetConsole().Log(
              attacker->GetName() + " пытается атаковать " +
              attacked->GetName() + " но " + on_cell->GetName() + " мешает!");
          return;
        }
      }
    }
  } else {
    HideAttackMenu();
    // not in attack range
    return;
  }
  HideAttackMenu();

  AttackCharacter(attacker, attacked, in_part);
}

void BattleField::TryAttackObject(Character* attacker, Object* who) {
  if (!who || is_battle_ended_) return;

  auto line_points = GetLineBetweenTwoObjects(attacker, who);

  if (line_points.size() >= attacker->GetStats().min_attack_range &&
      line_points.size() <=
          attacker->GetStats().max_attack_range)  // attack range check
  {
    for (std::pair<int, int> point : line_points) {
      Object* on_cell = cells_[point.first][point.second]->GetObjectOnCell();
      if (on_cell && on_cell != attacker) {
        if (on_cell->IsAttackable()) {
          AttackObject(attacker, on_cell);
          return;
        } else {
          GameState::GetGameState().GetConsole().Log(
              attacker->GetName() + " пытается атаковать " + who->GetName() +
              " но " + on_cell->GetName() + " мешает!");
          return;
        }
      }
    }
  } else {
    HideAttackMenu();
    return;
  }

  Character* character = dynamic_cast<Character*>(who);

  if (character) {
    if (dynamic_cast<PlayerCharacter*>(attacker))
      ShowAttackMenu(character);
    else {
      AttackCharacter(attacker, character, character->GetRandomBodyPart());
    }
  } else {
    AttackObject(attacker, who);
  }
}

void BattleField::AttackObject(Character* attacker, Object* who) {
  if (!who || is_battle_ended_) return;

  HideAttackMenu();
  if (attacker->GetStats().attack_count <= 0) {
    GameState::GetGameState().GetConsole().Log(
        attacker->GetName() + " пытается атаковать " + who->GetName() +
        " но у него нет очков удара!");

    return;
  }

  Character* character = dynamic_cast<Character*>(who);

  if (character) {
    AttackCharacter(attacker, character, character->GetRandomBodyPart());

  } else {
    attacker->GetStats().attack_count -= 1;
    GameState::GetGameState().GetConsole().Log(
        attacker->GetName() + " попадает в " + who->GetName() + " и наносит " +
        std::to_string(
            who->GetStats().CalcDamageForMe(attacker->CalcDamage())) +
        " урона");
    attacker->YouAttacker(who);
  }
}

void BattleField::AttackCharacter(Character* attacker, Character* attacked,
                                  BodyPart* in_part) {
  if (is_battle_ended_) return;
  HideAttackMenu();
  if (attacker->GetStats().attack_count <= 0) {
    GameState::GetGameState().GetConsole().Log(
        attacker->GetName() + " пытается атаковать " + attacked->GetName() +
        " но у него нет очков удара!");

    return;
  }

  attacker->GetStats().attack_count -= 1;

  GameState::GetGameState().GetConsole().Log(
      attacker->GetName() + " попадает в " + in_part->GetName() + " " +
      attacked->GetName() + " И наносит " +
      std::to_string(
          attacked->GetStats().CalcDamageForMe(attacker->CalcDamage())) +
      " урона");
  attacker->YouAttacker(attacked, in_part);
}

#include "battle_field.h"

#include "../Basic/game_state.h"
#include "../Basic/object_fabric.h"
#include "battle_cell.h"

#ifndef FSTREAM
#define FSTREAM
#include <fstream>
#endif

BattleField::BattleField(sf::RenderWindow& window, std::string map_file_path,
                         const std::pair<float, float> pos,
                         const std::string texture_file,
                         const sf::IntRect texture_rectangle)
    : Widget(window, pos,
             {kBattleFieldRectangleSizeScale_, kBattleFieldRectangleSizeScale_},
             texture_file, texture_rectangle) {
  CalcBattleCellSize();
  CreateCells(map_file_path);
}

BattleField::BattleField(sf::RenderWindow& window, std::string map_file_path,
                         Icon* icon, const std::pair<float, float> pos)
    : Widget(
          window, icon, pos,
          {kBattleFieldRectangleSizeScale_, kBattleFieldRectangleSizeScale_}) {
  CalcBattleCellSize();
  CreateCells(map_file_path);
}

BattleField::~BattleField() {}

void BattleField::Update() {}

void BattleField::Draw() {
  for (int i = 0; i < kCountBattleCells_; ++i) {
    for (int j = 0; j < kCountBattleCells_; ++j) {
      if (cells_[i][j]) {
        cells_[i][j]->Draw();
      }
    }
  }
}

void BattleField::CreateCells(std::string map_file_path) {
  ClearCells();
  ClearObjects();

  std::ifstream file(map_file_path);
  if (!file.is_open()) {
    cells_.resize(kCountBattleCells_);
    for (int i = 0; i < kCountBattleCells_; ++i) {
      cells_[i].resize(kCountBattleCells_);
      for (int j = 0; j < kCountBattleCells_; ++j) {
        Object* default_cell_object = ObjectFabric::GetObjectByChar('a');
        objects_on_field_.push_back(default_cell_object);

        float cell_pos_x =
            (kBorderBattleCellMargin_ - kBetweenBattleCellMargin_ +
             i * (battle_cell_size_ + kBetweenBattleCellMargin_));
        float cell_pos_y =
            (kBorderBattleCellMargin_ - kBetweenBattleCellMargin_ +
             j * (battle_cell_size_ + kBetweenBattleCellMargin_));
        cells_[i][j] =
            new BattleCell(*default_cell_object, nullptr,
                           GameState::GetGameState().GetWindow(),
                           std::pair(cell_pos_x, cell_pos_y),
                           std::pair(battle_cell_size_, battle_cell_size_));
      }
    }
    return;
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
      objects_on_field_.push_back(basic_object);

      float cell_pos_x = (kBorderBattleCellMargin_ - kBetweenBattleCellMargin_ +
                          i * (battle_cell_size_ + kBetweenBattleCellMargin_));
      float cell_pos_y = (kBorderBattleCellMargin_ - kBetweenBattleCellMargin_ +
                          j * (battle_cell_size_ + kBetweenBattleCellMargin_));

      cells_[i][j] = new BattleCell(
          *basic_object, nullptr, GameState::GetGameState().GetWindow(),
          std::pair(cell_pos_x, cell_pos_y),
          std::pair(battle_cell_size_, battle_cell_size_));
    }
  }

  int x, y;
  char symbol;
  while (file >> x >> y >> symbol) {
    if (x >= 0 && x < kCountBattleCells_ && y >= 0 && y < kCountBattleCells_) {
      Object* tmp_object = ObjectFabric::GetObjectByChar(symbol);
      if (tmp_object && cells_[x][y]) {
        objects_on_field_.push_back(tmp_object);
        cells_[x][y]->SetObjectOnCell(tmp_object);
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
      (kBattleFieldRectangleSizeScale_ - kBorderBattleCellMargin_ -
       (kBetweenBattleCellMargin_ * kCountBattleCells_)) /
      kCountBattleCells_;
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

    cells_[x][y]->SetObjectOnCell(nullptr);
    return true;
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

void BattleField::SetMovementController(MovementController* controller) {
  for (int i = 0; i < kCountBattleCells_; ++i) {
    for (int j = 0; j < kCountBattleCells_; ++j) {
      if (cells_[i][j] != nullptr) {
        cells_[i][j]->SetMovementController(controller);
      }
    }
  }
}

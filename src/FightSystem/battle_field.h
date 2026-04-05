#ifndef BATTLE_FIELD
#define BATTLE_FIELD

#include "../Visual/widget.h"
#include "battle_cell.h"

#ifndef VECTOR
#include <vector>
#define VECTOR
#endif  // !VECTOR

class BattleField : public Widget {
 public:
  BattleField(sf::RenderWindow& window, std::string map_file_path,
              const std::pair<float, float> pos, const std::string texture_file,
              const sf::IntRect texture_rectangle);
  BattleField(sf::RenderWindow& window, std::string map_file_path, Icon* icon,
              const std::pair<float, float> pos);

  ~BattleField();

  void Update();
  void Draw();

  // Movement support
  int GetWidth() const;
  int GetHeight() const;
  Object* GetCellObject(int x, int y);
  bool GetCharacterPosition(Object* character, int& out_x, int& out_y);
  bool PlaceObjectOnCell(Object* object, int x, int y);
  bool RemoveObjectFromCell(int x, int y);
  void HighlightCell(int x, int y, sf::Color color);
  void ClearAllHighlights();
  void GetCellCoordinates(BattleCell* cell, int& out_x, int& out_y);
  BattleCell* GetCell(int x, int y);
  BattleCell* GetCellFromButton(Button* button);
  void SetMovementController(MovementController* controller);

 private:
  static inline const float kBetweenBattleCellMargin_ = 1.f;
  static inline const float kBorderBattleCellMargin_ = 10.f;
  static inline const float kBattleFieldRectangleSizeScale_ = 810;
  static inline const int kCountBattleCells_ = 16;
  float battle_cell_size_;

  std::vector<std::vector<BattleCell*>> cells_;
  std::vector<Object*> objects_on_field_;

  void CreateCells(std::string map_file_path);
  void ClearCells();
  void ClearObjects();
  void CalcBattleCellSize();
};
#endif  // !BATTLE_FIELD

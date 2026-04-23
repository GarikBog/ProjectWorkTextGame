#ifndef BATTLE_FIELD
#define BATTLE_FIELD

#include "../Visual/widget.h"
#include "battle_cell.h"

#ifndef VECTOR
#include <vector>
#define VECTOR
#endif  // !VECTOR

#include "../Visual/body_part_widget.h"
#include "../Visual/character_info_widget.h"
#include "body_part.h"

class BattleField : public Widget, public IButtonOwner {
 public:
  BattleField(sf::RenderWindow& window, std::string map_file_path,
              const std::pair<float, float> pos, const std::string texture_file,
              const sf::IntRect texture_rectangle);
  BattleField(sf::RenderWindow& window, std::string map_file_path, Icon* icon,
              const std::pair<float, float> pos);

  ~BattleField();

  void AddObject(Object* object);
  void Update();
  void Draw() override;
  void SetVisibility(bool is_visible_now) override;

  int GetWidth() const;
  int GetHeight() const;
  CharacterInfoWidget* GetInfoWidget();
  Object* GetCellObject(int x, int y);
  bool GetCharacterPosition(Object* character, int& out_x, int& out_y);
  std::vector<std::pair<int, int>> GetLineBetweenTwoCells(int x1, int y1,
                                                          int x2, int y2);
  std::vector<std::pair<int, int>> GetLineBetweenTwoObjects(Object* first,
                                                            Object* second);
  bool PlaceObjectOnCell(Object* object, int x, int y);
  bool RemoveObjectFromCell(int x, int y);
  void ObjectWasDestroyed(Object* object);
  void ReplaceMe(Object* object, Object* replace_with);
  void HighlightCell(int x, int y, sf::Color color);
  void ClearAllHighlights();
  void GetCellCoordinates(BattleCell* cell, int& out_x, int& out_y);
  BattleCell* GetCell(int x, int y);
  BattleCell* GetCellFromButton(Button* button);
  void SetMovementController(MovementController* controller);

  void TryAttackCharacter(Character* attacker, Character* attacked,
                          BodyPart* in_part);
  void AttackCharacter(Character* attacker, Character* attacked,
                       BodyPart* in_part);
  void TryAttackObject(Character* attacker, Object* who);
  void AttackObject(Character* attacker, Object* who);
  void ShowAttackMenu(Character* character);
  void HideAttackMenu();

  void EndBattle();
  bool IsBattleEnded() const;

 private:
  static inline const float kBetweenBattleCellMargin_ = 1.f;
  static inline const float kBorderBattleCellMargin_ = 5.f;
  static inline const float kBattleFieldRectangleSizeScale_ = 810.f;
  static inline const int kCountBattleCells_ = 16;
  static inline const float kMarginBetweenFieldAndButtons = 8.f;
  static inline const float kBodyPartWidgetHeight = 600.f;
  static inline const float kBodyPartWidgetWidth = 400.f;
  static inline const float kNextTurnButtonWidth = 400.f;
  static inline const float kNextTurnButtonHeight = 200.f;
  float battle_cell_size_;
  bool is_battle_ended_;

  std::vector<std::vector<BattleCell*>> cells_;
  std::vector<Object*> objects_on_field_;

  CharacterInfoWidget character_info_widget_;
  Button end_turn_button_;
  BodyPartWidget body_choise_menu_;

  void CreateCells(std::string map_file_path);
  void ClearCells();
  void ClearObjects();
  void CalcBattleCellSize();

  void ButtonIsPressed(Button* button) override;
};
#endif  // !BATTLE_FIELD

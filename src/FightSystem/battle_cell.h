

#ifndef BATTLE_CELL
#define BATTLE_CELL

#include "object.h"

class BattleCell {
 public:
  Object* SetObjectOnCell(Object* object);

  Object* GetObjectOnCell();

  BattleCell(Object& basic_object);

 private:
  Object* object_on_cell_;
  Object& basic_object_on_cell_;
};

#endif

#ifndef OBJECT
#define OBJECT

#include "../Visual/icon.h"

class Object {
 public:
  virtual bool StayOn(Object* other_object);
  virtual bool MoveFrom(Object* other_object);
  virtual float GetMovementCost() const;
  bool IsAttackable() const;

  Object(std::string texture_file, sf::IntRect texture_rect);
  Icon* GetIcon();

 private:
  Icon icon_;
  bool is_walkable_;
  bool is_attackable_;
};

#endif  // !OBJECT

#include "object.h"

bool Object::StayOn(Object* other_object) { return is_walkable_; }

bool Object::MoveFrom(Object* other_object) { return true; }

bool Object::IsAttackable() const { return is_attackable_; }

Object::Object(std::string texture_file, sf::IntRect texture_rect)
    : icon_(texture_file, texture_rect) {
  is_walkable_ = true;
  is_attackable_ = true;
}

Icon* Object::GetIcon() { return &icon_; }

float Object::GetMovementCost() const { return 1.0f; }

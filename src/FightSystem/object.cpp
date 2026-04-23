#include "object.h"

#include "../Basic/game_state.h"
#include "battle_field.h"
#include "body_part.h"
#include "movement_controller.h"

bool Object::StayOn(Object* other_object) { return IsWalkable(); }

bool Object::MoveFrom(Object* other_object) { return true; }

bool Object::IsAttackable() const { return is_attackable_; }

bool Object::IsWalkable() const { return is_walkable_; }

bool Object::YouAttacked(Character* by_who, int damage) {
  if (!stats_->GetDamage(damage) && is_attackable_) {
    Destroy();
    return false;
  }
  return true;
}

void Object::Check() {
  if (GameState::GetGameState()
          .GetCurrentBattle()
          ->GetLineBetweenTwoObjects(
              this, GameState::GetGameState().GetPlayer().GetPlayerCharacter())
          .size() > 2) {
    GameState::GetGameState().GetConsole().Log("Dexyan пытается осмотреть " +
                                               name_ + " но он слишком далеко");

  } else
    GameState::GetGameState().GetConsole().Log(
        "Dexyan осматривает " + name_ + " но не находит ничего необычного");
}

BattleStats& Object::GetStats() { return *stats_; }

void Object::ChangeDefensive(float count) { stats_->defensive += count; }

void Object::ChangeDamage(int damage) { stats_->damage += damage; }

void Object::Destroy() {
  std::cout << "Destroy\n";
  GameState::GetGameState().GetCurrentBattle()->ObjectWasDestroyed(this);
  if (GameState::GetGameState().GetMovementController())
    GameState::GetGameState().GetMovementController()->UnregisterObject(this);
  delete this;
}

std::string Object::GetName() const {
  // asd
  return name_;
}

Object::Object(std::string texture_file, sf::IntRect texture_rect,
               std::string name, BattleStats& battle_stats, bool is_walkable,
               bool is_attackable)
    : icon_(texture_file, texture_rect),
      name_(name),
      is_walkable_(is_walkable),
      is_attackable_(is_attackable),
      stats_(new BattleStats(battle_stats)),
      is_self_made_stats_(true) {
  type_ = kStaticObject;
}

Icon* Object::GetIcon() { return &icon_; }

CharactersTypes Object::GetType() const { return type_; }

Object::~Object() {
  if (is_self_made_stats_) delete stats_;
}

float Object::GetMovementCost() const {
  if (!is_walkable_) return 99999.f;
  return 1.f;
}

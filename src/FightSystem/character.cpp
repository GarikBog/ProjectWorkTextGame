#include "character.h"

#include "../Basic/game_state.h"

Character::Character(std::string texture_file, sf::IntRect texture_rect,
                     std::string name, BodyPart&& torso,
                     BattleStats& battle_stats)
    : Object(texture_file, texture_rect, name, battle_stats),
      torso_(std::move(torso)) {
  is_walkable_ = false;
  is_attackable_ = false;
  type_ = kEnemyCharacter;
}

Character::~Character() {}

int Character::GetCurrentStamina() const { return stats_->current_stamina; }

void Character::SetCurrentStamina(float stamina) {
  stats_->current_stamina = stamina;
}

bool Character::YouAttacked(Character* by_who, BodyPart* in_part) {
  stats_->GetDamage(in_part->GetLastDamage());

  if (stats_->health <= 0 || !torso_.isAlive()) {
    Destroy();
    return false;
  }
  return true;
}

void Character::YouAttacker(Character* who, BodyPart* in_part) {
  in_part->GetDamage(who->GetStats().CalcDamageForMe(CalcDamage()));
  who->YouAttacked(this, in_part);
}

void Character::YouAttacker(Object* what) {
  what->YouAttacked(this, what->GetStats().CalcDamageForMe(CalcDamage()));
}

std::vector<BodyPart*> Character::GetBodyParts() {
  return std::vector<BodyPart*>{&torso_};
}

BodyPart* Character::GetRandomBodyPart() { return &torso_; }

bool Character::YouAttacked(Character* by_who, int damage) {
  return Object::YouAttacked(by_who, damage);
}

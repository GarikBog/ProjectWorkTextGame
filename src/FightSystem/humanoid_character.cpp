#include "humanoid_character.h"

#include "../Basic/game_state.h"
HumanoidCharacter::HumanoidCharacter(std::string texture_file,
                                     sf::IntRect texture_rect, std::string name,
                                     std::vector<BodyPart> body_parts,
                                     BattleStats& battle_stats)
    : Character(texture_file, texture_rect, name, std::move(body_parts[0]),
                battle_stats),
      head_(std::move(body_parts[1])),
      left_hand_(std::move(body_parts[2])),
      right_hand_(std::move(body_parts[3])),
      left_leg_(std::move(body_parts[4])),
      right_leg_(std::move(body_parts[5])) {
  is_attackable_ = true;
  type_ = kEnemyCharacter;
}

HumanoidCharacter::~HumanoidCharacter() {}

std::vector<BodyPart*> HumanoidCharacter::GetBodyParts() {
  return std::vector<BodyPart*>{&torso_,      &head_,     &left_hand_,
                                &right_hand_, &left_leg_, &right_leg_};
}

bool HumanoidCharacter::YouAttacked(Character* by_who, BodyPart* in_part) {
  if (Character::YouAttacked(by_who, in_part)) {
    if (!head_.isAlive()) {
      Destroy();
      return false;
    }
  } else
    return false;
  return true;
}

int HumanoidCharacter::CalcDamage() {
  return stats_->damage *
         (left_hand_.GetHealthPercent() + right_hand_.GetHealthPercent()) / 2;
}

BodyPart* HumanoidCharacter::GetRandomBodyPart() {
  int rand_part = rand() % 6;
  switch (rand_part) {
    case (0):
      return &head_;
    case (1):
      return &right_hand_;
    case (2):
      return &left_hand_;
    case (3):
      return &right_leg_;
    case (4):
      return &left_leg_;
    default:
      return &torso_;
  }
}

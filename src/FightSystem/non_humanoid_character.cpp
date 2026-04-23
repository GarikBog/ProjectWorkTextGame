#include "non_humanoid_character.h"

NonHumanoidCharacter::NonHumanoidCharacter(std::string texture_file,
                                           sf::IntRect texture_rect,
                                           std::string name,
                                           std::vector<BodyPart> body_parts,
                                           BattleStats& battle_stats)
    : Character(texture_file, texture_rect, name, std::move(body_parts[0]),
                battle_stats) {
  body_parts.erase(body_parts.begin());
  body_parts_ = body_parts;
  is_attackable_ = true;
  is_walkable_ = false;
}

NonHumanoidCharacter::~NonHumanoidCharacter() {}

std::vector<BodyPart*> NonHumanoidCharacter::GetBodyParts() {
  std::vector<BodyPart*> parts = {&torso_};

  for (BodyPart& part : body_parts_) {
    parts.push_back(&part);
  }

  return parts;
}

int NonHumanoidCharacter::CalcDamage() { return stats_->damage; }

BodyPart* NonHumanoidCharacter::GetRandomBodyPart() {
  if (body_parts_.empty()) return &torso_;
  int rand_part = rand() % body_parts_.size() + 1;

  if (rand_part == body_parts_.size()) return &torso_;
  return &body_parts_[rand_part];
}

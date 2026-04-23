#ifndef HUMANOID_CHARACTER
#define HUMANOID_CHARACTER

#include "character.h"

class HumanoidCharacter : public Character {
 public:
  HumanoidCharacter(std::string texture_file, sf::IntRect texture_rect,
                    std::string name, std::vector<BodyPart> body_parts,
                    BattleStats& battle_stats = BattleStats());
  virtual ~HumanoidCharacter() override;
  std::vector<BodyPart*> GetBodyParts() override;

  virtual bool YouAttacked(Character* by_who, BodyPart* in_part) override;

  virtual int CalcDamage() override;
  virtual BodyPart* GetRandomBodyPart() override;

 protected:
  BodyPart head_;
  BodyPart left_hand_;
  BodyPart right_hand_;
  BodyPart left_leg_;
  BodyPart right_leg_;
};

#endif  // !BATTLE_FIELD

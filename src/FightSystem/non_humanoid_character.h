#ifndef NON_HUMANOID_CHARACTER
#define NON_HUMANOID_CHARACTER

#include "../FightSystem/character.h"

class NonHumanoidCharacter : public Character {
 public:
  NonHumanoidCharacter(std::string texture_file, sf::IntRect texture_rect,
                       std::string name, std::vector<BodyPart> body_parts,
                       BattleStats& battle_stats);
  virtual ~NonHumanoidCharacter();
  std::vector<BodyPart*> GetBodyParts() override;

  virtual int CalcDamage() override;
  virtual BodyPart* GetRandomBodyPart() override;

 protected:
  std::vector<BodyPart> body_parts_;
};

#endif  // !NHC

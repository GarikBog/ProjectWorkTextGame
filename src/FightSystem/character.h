#ifndef CHARACTER
#define CHARACTER

#include "body_part.h"
#include "object.h"

class Character : public Object {
 public:
  Character(std::string texture_file, sf::IntRect texture_rect,
            std::string name, BodyPart&& torso, BattleStats& battle_stats);
  virtual ~Character() override;
  int GetCurrentStamina() const;
  void SetCurrentStamina(float stamina);

  virtual bool YouAttacked(Character* by_who, BodyPart* in_part);
  virtual void YouAttacker(Character* who, BodyPart* in_part);
  virtual void YouAttacker(Object* what);

  virtual int CalcDamage() = 0;
  virtual std::vector<BodyPart*> GetBodyParts();
  virtual BodyPart* GetRandomBodyPart();

 protected:
  BodyPart torso_;

 private:
  bool YouAttacked(Character* by_who, int damage) override;
};

#endif  //

#ifndef OBJECT
#define OBJECT
#include "../Basic/stats.h"
#include "../Visual/icon.h"
enum CharactersTypes {
  kStaticObject,
  kInteractableObject,
  kPlayerCharacter,
  kEnemyCharacter,
};

class Character;

class Object {
 public:
  virtual bool StayOn(Object* other_object);
  virtual bool MoveFrom(Object* other_object);
  virtual float GetMovementCost() const;
  bool IsAttackable() const;
  bool IsWalkable() const;
  virtual bool YouAttacked(Character* by_who, int damage);
  virtual void Check();
  BattleStats& GetStats();
  void ChangeDefensive(float count);
  void ChangeDamage(int damage);

  virtual void Destroy();
  std::string GetName() const;

  Object(std::string texture_file, sf::IntRect texture_rect,
         std::string name = "", BattleStats& battle_stats = BattleStats(),
         bool is_walkable = true, bool is_attackable = true);
  Icon* GetIcon();

  CharactersTypes GetType() const;

  virtual ~Object();

 protected:
  bool is_walkable_;
  bool is_attackable_;
  bool is_self_made_stats_;
  CharactersTypes type_;
  std::string name_;
  BattleStats* stats_;

 private:
  Icon icon_;
};

#endif  // !OBJECT

#ifndef BODY_PART
#define BODY_PART

#include <string>

#include "../Visual/icon.h"

class BodyPart {
 public:
  BodyPart(std::string name, int max_health, std::string texture_file_path,
           sf::IntRect texture_rectangle);
  BodyPart(BodyPart&& other) noexcept;
  BodyPart(BodyPart& other) noexcept;
  BodyPart& operator=(BodyPart& other) noexcept;
  BodyPart& operator=(BodyPart&& other) noexcept;

  std::string GetName() const;
  std::string GetHealthInString() const;
  Icon& GetIcon();

  void GetDamage(int damage);
  int GetHealth() const;
  float GetHealthPercent() const;
  int GetLostHealth() const;
  int GetLastDamage() const;
  bool isAlive() const;

 private:
  int max_health_;
  int health_;
  std::string name_;
  Icon body_icon_;

 private:
  int last_damage = 0;
};
#endif  // !BODY_PART

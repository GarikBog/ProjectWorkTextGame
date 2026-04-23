#ifndef PLAYER_CHARACTER
#define PLAYER_CHARACTER
#include "humanoid_character.h"

class PlayerCharacter : public HumanoidCharacter {
 public:
  PlayerCharacter(std::string texture_file, sf::IntRect texture_rect,
                  std::vector<BodyPart> body_parts);
  ~PlayerCharacter();
};

#endif  // !PLAYER_CHARACTER

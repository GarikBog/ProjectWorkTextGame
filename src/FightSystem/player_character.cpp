#include "player_character.h"

#include "../Basic/game_state.h"
PlayerCharacter::PlayerCharacter(std::string texture_file,
                                 sf::IntRect texture_rect,
                                 std::vector<BodyPart> body_parts)
    : HumanoidCharacter(texture_file, texture_rect, "Dexyan", body_parts) {
  type_ = kPlayerCharacter;
  if (is_self_made_stats_) {
    is_self_made_stats_ = false;
    delete stats_;
  }

  stats_ = &(GameState::GetGameState().GetPlayer().GetBattleStats());
  GameState::GetGameState().GetPlayer().SetPlayerCharacter(this);
}

PlayerCharacter::~PlayerCharacter() {
  GameState::GetGameState().GetPlayer().SetPlayerCharacter(nullptr);
  GameState::GetGameState().GetPlayer().Kill();
  GameState::GetGameState().PlayerDieInBattle();
}

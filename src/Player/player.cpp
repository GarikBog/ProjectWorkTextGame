#include "player.h"

#include <fstream>
#include <iostream>
#include <sstream>

void Player::SetPlayerCharacter(PlayerCharacter* player_character) {
  if (!player_character) {
    player_character_ = nullptr;
    return;
  }
  if (player_character_)
    throw std::exception("Created 2 PlayerCharacter in one time!");

  player_character_ = player_character;
}

Player::Player()
    : battle_stats_(0, 0, 0, 1), player_character_(nullptr), is_alive(true) {
  std::ifstream file(ASSETS_PATH "/characters_stats/player.txt");

  if (!file.is_open()) {
    throw std::exception("Could not open player stats file!");
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty()) continue;
    std::istringstream iss(line);
    std::string key, value;
    iss >> key >> value;
    if (key == "max_health") {
      battle_stats_.health = battle_stats_.max_health = std::stoi(value);
    } else if (key == "stamina") {
      battle_stats_.current_stamina = battle_stats_.stamina = std::stoi(value);
    }
  }

  file.close();
}

BattleStats& Player::GetBattleStats() { return battle_stats_; }

PlayerCharacter* Player::GetPlayerCharacter() { return player_character_; }

Inventory& Player::GetInventory() { return inventory_; }

void Player::ChangeDefensive(float count) { battle_stats_.defensive += count; }
void Player::ChangeDamage(int count) { battle_stats_.damage += count; }

bool Player::IsAlive() const { return is_alive && battle_stats_.health > 0; }

void Player::Kill() { is_alive = false; }

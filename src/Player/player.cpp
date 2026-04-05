#include "player.h"

#include <fstream>
#include <iostream>
#include <sstream>

Player::Player() {
  std::ifstream file(ASSETS_PATH "/characters_stats/player.txt");

  if (!file.is_open()) {
    throw std::exception("Could not open player stats file!");
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty()) continue;

    std::istringstream iss(line);
    std::string key, value;
    if (key == "max_health") {
      stats.health = stats.max_health = std::stoi(value);
    } else if (key == "stamina") {
      stats.current_stamina = stats.stamina = std::stoi(value);
    }
  }

  file.close();
}

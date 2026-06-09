#include "object_fabric.h"

#include <fstream>
#include <sstream>

#include "../FightSystem/lootable_object.h"
#include "../FightSystem/non_humanoid_character.h"
#include "../FightSystem/player_character.h"

Object* ObjectFabric::GetObjectByChar(const char ch) {
  switch (ch) {
      // dexyan
    case ('d'): {
      auto body_parts =
          GetBodyPartsFromFile(ASSETS_PATH + (object_body_paths_.at("human")));
      return new PlayerCharacter(ASSETS_PATH + object_textures_.at("pudge"),
                                 sf::IntRect(0, 0, 800, 800), body_parts);
    }

      // enemis NON HUM
    case ('s'): {
      auto body_parts =
          GetBodyPartsFromFile(ASSETS_PATH + (object_body_paths_.at("slime")));
      return new NonHumanoidCharacter(
          ASSETS_PATH + object_textures_.at("slime"),
          sf::IntRect(0, 0, 800, 800), "Slime", body_parts,
          BattleStats(20, 5, 0, 1, 5, 0));
    }
    case ('a'): {
      auto body_parts = GetBodyPartsFromFile(
          ASSETS_PATH + (object_body_paths_.at("serafim")));
      return new NonHumanoidCharacter(
          ASSETS_PATH + object_textures_.at("serafim"),
          sf::IntRect(0, 0, 800, 800), "Serafim", body_parts,
          BattleStats(150, 1, 0, 4, 10, 0.3));
    }

      // enemis HUM
    case ('m'): {
      auto body_parts =
          GetBodyPartsFromFile(ASSETS_PATH + (object_body_paths_.at("human")));
      return new HumanoidCharacter(ASSETS_PATH + object_textures_.at("creep"),
                                   sf::IntRect(0, 0, 800, 800), "Creep",
                                   body_parts,
                                   BattleStats(50, 3, 0, 2, 5, 0.5));
    }
    case ('r'): {
      auto body_parts =
          GetBodyPartsFromFile(ASSETS_PATH + (object_body_paths_.at("human")));
      return new HumanoidCharacter(
          ASSETS_PATH + object_textures_.at("range_creep"),
          sf::IntRect(0, 0, 800, 800), "RangeCreep", body_parts,
          BattleStats(50, 5, 3, 5, 5, 0.1));
    }

    // objects
    case ('k'):
      return new Object(ASSETS_PATH + object_textures_.at("rock"),
                        sf::IntRect(0, 0, 800, 800), "rock", BattleStats(15),
                        false, true);
    case ('l'):
      return new Object(ASSETS_PATH + object_textures_.at("lava"),
                        sf::IntRect(0, 0, 250, 250), "lava",
                        BattleStats(9999999999999), false, true);
    case ('b'):
      return new Object(ASSETS_PATH + object_textures_.at("bricks"),
                        sf::IntRect(0, 0, 512, 512), "brick wall",
                        BattleStats(9999999999999), false, false);
    case ('c'):
      return new LootableObject(ASSETS_PATH + object_textures_.at("chest"),
                                sf::IntRect(0, 0, 320, 320), ASSETS_PATH "",
                                "chest", BattleStats(5));
    default:
      return new Object(ASSETS_PATH + object_textures_.at("ground"),
                        sf::IntRect(0, 0, 800, 800));
      ;
  }
}
std::vector<BodyPart> ObjectFabric::GetBodyPartsFromFile(
    std::string file_path) {
  std::vector<BodyPart> parts;
  std::ifstream current_file(file_path);
  std::string line;
  std::string current_texture_file;

  if (!current_file.is_open())
    throw std::exception("Can't open body part file!");
  while (std::getline(current_file, line)) {
    if (!line.empty() && line.back() == '\r') line.pop_back();

    if (line.empty()) continue;

    if (line.find('/') != std::string::npos) {
      current_texture_file = ASSETS_PATH + line;
    } else {
      std::istringstream iss(line);
      std::string name;
      int max_health, texture_x, texture_y, texture_w, texture_h;

      if (iss >> name >> max_health >> texture_x >> texture_y >> texture_w >>
          texture_h) {
        parts.push_back(
            BodyPart(name, max_health, current_texture_file,
                     sf::IntRect(texture_x, texture_y, texture_w, texture_h)));
      }
    }
  }
  current_file.close();
  return parts;
}

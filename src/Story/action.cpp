#include "action.h"

#include <fstream>
#include <sstream>

#include "../Basic/game_state.h"

Action::Action(
    std::string description,
    std::map<std::string, std::vector<std::pair<std::string, float>>>&&
        conditions,
    std::string outcome_event_file, PlayerBehavior&& behavior_diff,
    std::string battle_file_path)
    : description(description),
      conditions(conditions),
      outcome_event_file(outcome_event_file),
      behavior_diff(behavior_diff),
      battle_file_path(battle_file_path) {}

void Action::Do() {
  if (!battle_file_path.empty() && battle_file_path != "NONE")
    GameState::GetGameState().StartBattle(battle_file_path);
  GameState::GetGameState().GetPlayerBehavior() += behavior_diff;
  GameState::GetGameState().GetCurrentBigEvent()->LoadSmallEventFromFile(
      outcome_event_file);
}

bool Action::CheckCondition() {
  for (auto& pair : conditions) {
    for (auto condition : pair.second) {
      if (!CheckThisCondition(pair.first, condition.first, condition.second))
        return false;
    }
  }
  return true;
}

std::string Action::GetDescription() const { return description; }

const std::vector<std::string>& Action::GetPossibleWords() const {
  return possible_words_;
}

bool Action::CheckThisCondition(std::string type, std::string condition,
                                float value) {
  if (type == "HAVE") {
    return GameState::GetGameState().GetPlayer().GetInventory().DoesHasItem(
        condition);
  } else if (type == "STATS_MORE") {
    return GameState::GetGameState().GetPlayer().GetStatByString(condition) >=
           value;
  } else if (type == "STATS_LESS") {
    return GameState::GetGameState().GetPlayer().GetStatByString(condition) <=
           value;
  }
}
Action* Action::CreateActionFromFile(std::string file) {
  std::ifstream input(file);
  if (!input.is_open()) {
    input.open(ASSETS_PATH + file);
    if (!input.is_open()) throw std::runtime_error("Can't open Action" + file);
  }

  std::string description;
  std::string outcome_event_file;
  std::string battle_file_path;
  std::map<std::string, std::vector<std::pair<std::string, float>>> conditions;
  PlayerBehavior behavior_diff;
  std::vector<std::string> possible_words;

  std::string line;

  while (std::getline(input, line)) {
    std::istringstream iss(line);
    std::string directive;
    if (!(iss >> directive)) continue;

    if (directive == "DESCRIPTION") {
      std::getline(iss, description);
      size_t start = description.find_first_not_of(" \t");
      if (start != std::string::npos) {
        description = description.substr(start);
      } else {
        description.clear();
      }
    } else if (directive == "OUTCOME") {
      iss >> outcome_event_file;
    } else if (directive == "BATTLE") {
      iss >> battle_file_path;
    } else if (directive == "CONDITION") {
      std::string type, condition;
      float value = 0.0f;
      if (iss >> type >> condition) {
        if (!(iss >> value)) {
          value = 0.0f;
        }
        conditions[type].emplace_back(condition, value);
      }
    } else if (directive == "BEHAVIOR") {
      std::string field;
      float value = 0.0f;
      if (iss >> field >> value) {
        if (field == "HOSTILITY") {
          behavior_diff.addHostility(value);
        } else if (field == "EXPLORATION") {
          behavior_diff.addExploration(value);
        } else if (field == "MERCY") {
          behavior_diff.addMercy(value);
        } else if (field == "HONOR") {
          behavior_diff.addHonor(value);
        } else if (field == "ENRICHMENT") {
          behavior_diff.addEnrichment(value);
        } else if (field == "HONESTY") {
          behavior_diff.addHonesty(value);
        }
      }
    } else if (directive == "WORD") {
      std::string word;
      if (iss >> word) {
        possible_words.push_back(word);
      }
    }
  }

  Action* action =
      new Action(description, std::move(conditions), outcome_event_file,
                 std::move(behavior_diff), battle_file_path);
  action->possible_words_ = std::move(possible_words);
  return action;
}

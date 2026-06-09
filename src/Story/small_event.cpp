#include "small_event.h"

#include <fstream>
#include <sstream>
#include <string>

#include "../Basic/game_state.h"

void SmallEvent::PrintDiscriptionToConsole() {
  if (description_.empty()) return;
  const size_t max_len =
      GameState::GetGameState().GetConsole().GetMaxInputLenght();
  std::istringstream iss(description_);
  std::string word;
  std::string current_line;
  while (iss >> word) {
    if (word.size() > max_len) {
      if (!current_line.empty()) {
        GameState::GetGameState().GetConsole().Log(current_line);
        current_line.clear();
      }
      GameState::GetGameState().GetConsole().Log(word);
      continue;
    }
    if (current_line.empty()) {
      current_line = word;
    } else if (current_line.size() + 1 + word.size() <= max_len) {
      current_line += " " + word;
    } else {
      GameState::GetGameState().GetConsole().Log(current_line);
      current_line = word;
    }
  }
  if (!current_line.empty()) {
    GameState::GetGameState().GetConsole().Log(current_line);
  }
}

void SmallEvent::PrintPossibleActions() {
  GameState::GetGameState().GetConsole().Log("Что ты будешь делать?");
  int index = 1;
  for (Action* action : possible_actions_) {
    if (action && action->CheckCondition()) {
      GameState::GetGameState().GetConsole().Log(
          std::to_string(index++) + ". " + action->GetDescription());
    }
  }
}

SmallEvent::SmallEvent(std::string description, std::string background,
                       std::vector<Action*>&& actions)
    : description_(description),
      background_name_(background),
      possible_actions_(actions) {}

void SmallEvent::ShowEvent() {
  GameState::GetGameState().LoadBackground(background_name_);
  PrintDiscriptionToConsole();
  PrintPossibleActions();
}

SmallEvent* SmallEvent::GetSmallEventFromFile(std::string file_path) {
  std::ifstream input(file_path);
  if (!input.is_open()) {
    input.open(ASSETS_PATH + file_path);
    if (!input.is_open())
      throw std::runtime_error("Can't open Small event " + file_path);
  }

  std::string description;
  std::string background;
  std::vector<Action*> actions;

  std::string line;
  while (std::getline(input, line)) {
    if (line.empty()) continue;

    std::istringstream iss(line);
    std::string directive;
    if (!(iss >> directive)) continue;

    if (directive == "DESCRIPTION") {
      std::string rest;
      std::getline(iss, rest);
      size_t start = rest.find_first_not_of(" \t");
      if (start != std::string::npos) {
        description = rest.substr(start);
      }
    } else if (directive == "BACKGROUND") {
      iss >> background;
    } else if (directive == "ACTION") {
      std::string action_file;
      if (iss >> action_file) {
        Action* action = Action::CreateActionFromFile(action_file);
        if (action) {
          actions.push_back(action);
        }
      }
    }
  }

  return new SmallEvent(description, background, std::move(actions));
}

SmallEvent::~SmallEvent() {
  for (Action* action : possible_actions_) {
    delete action;
  }
}

const std::vector<Action*>& SmallEvent::GetAllPossibleActions() const {
  return possible_actions_;
}

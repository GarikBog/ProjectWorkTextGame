#include "storyteller.h"

#include <fstream>
#include <sstream>

std::vector<StoryTypes> StoryTeller::GetDesiredTypes() { return {kExplorer}; }
StoryTypes StoryTeller::GetTypeByName(std::string name) {
  if (name == "Battle") return kBattle;
  if (name == "Explorer") return kExplorer;
  throw std::runtime_error("Unknown StoryType: " + name);
}

void StoryTeller::LoadStoriesFromFile(std::string file_path) {
  std::ifstream input(file_path);
  if (!input.is_open()) {
    input.open(ASSETS_PATH + file_path);
    if (!input.is_open()) return;
  }

  std::string line;

  enum ParseState { kNone, kEvents, kTypes, kUnique };
  ParseState state = kNone;

  Story* current_story = nullptr;

  while (std::getline(input, line)) {
    if (line.empty()) continue;

    if (line[0] == '#') continue;

    std::istringstream iss(line);
    std::string directive;
    if (!(iss >> directive)) continue;

    if (directive == "STORY") {
      current_story = new Story{};
      state = kEvents;
    } else if (directive == "TYPE") {
      state = kTypes;
    } else if (directive == "UNIQUE") {
      if (current_story) {
        current_story->is_unique = true;
      }
      state = kNone;
    } else if (directive == "REPEATED") {
      if (current_story) {
        current_story->is_unique = false;
      }
      state = kNone;
    } else {
      if (state == kEvents && current_story) {
        current_story->events.push(directive);
      } else if (state == kTypes && current_story) {
        StoryTypes type = GetTypeByName(directive);
        current_story->types.push_back(type);
        stories_[type].push_back(current_story);
      }
    }
  }
}

std::string StoryTeller::GetNextBigEvent() {
  std::vector<StoryTypes> desired_types = GetDesiredTypes();

  std::vector<Story*> candidates;

  for (Story* story : played_stories_) {
    candidates.push_back(story);
  }

  for (StoryTypes type : desired_types) {
    for (Story* story : stories_[type]) {
      candidates.push_back(story);
    }
  }

  if (candidates.empty()) {
    for (auto& pair : stories_) {
      for (Story* story : pair.second) {
        candidates.push_back(story);
      }
    }
  }

  if (candidates.empty()) {
    // DO LOGIC EMPTY EVENTS
    return "";
  }

  std::uniform_int_distribution<size_t> dist(0, candidates.size() - 1);
  Story* selected = candidates[dist(gen_)];

  bool from_stories = false;
  for (StoryTypes type : desired_types) {
    auto& vec = stories_[type];
    auto it = std::find(vec.begin(), vec.end(), selected);
    if (it != vec.end()) {
      from_stories = true;
      if (selected->is_unique) {
        vec.erase(it);
      }
    }
  }

  if (from_stories && selected->is_unique) {
    for (auto& pair : stories_) {
      auto& vec = pair.second;
      vec.erase(std::remove(vec.begin(), vec.end(), selected), vec.end());
    }
    played_stories_.push_back(selected);
  }

  std::string event;
  event = selected->events.front();
  if (selected->is_unique) {
    selected->events.pop();
  }

  if (selected->events.empty()) {
    if (!selected->is_unique) {
      for (auto& pair : stories_) {
        auto& vec = pair.second;
        vec.erase(std::remove(vec.begin(), vec.end(), selected), vec.end());
      }
      delete selected;
    } else {
      auto it =
          std::find(played_stories_.begin(), played_stories_.end(), selected);
      if (it != played_stories_.end()) {
        played_stories_.erase(it);
      }
      delete selected;
    }
  }

  return event;
}

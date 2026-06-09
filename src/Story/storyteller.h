#ifndef STORYTELLER
#define STORYTELLER
#include <map>
#include <queue>
#include <random>
#include <string>
#include <vector>

enum StoryTypes { kBattle, kExplorer };

class StoryTeller {
 public:
  StoryTeller::StoryTeller() : gen_(std::random_device{}()) {}
  std::string GetNextBigEvent();

  void LoadStoriesFromFile(std::string file_path);

 private:
  struct Story {
    std::vector<StoryTypes> types;
    std::queue<std::string> events;
    bool is_unique;
  };

  std::vector<Story*> played_stories_;
  std::map<StoryTypes, std::vector<Story*>> stories_;

  StoryTypes GetTypeByName(std::string name);

  std::vector<StoryTypes> GetDesiredTypes();

  std::mt19937 gen_;
};

#endif  // !STORYTELLER

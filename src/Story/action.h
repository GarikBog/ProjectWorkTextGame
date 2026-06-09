#ifndef ACTION
#define ACTION
#include <map>
#include <string>
#include <vector>

#include "../Basic/player_behavior.h"

struct Action {
 public:
  Action(std::string description,
         std::map<std::string, std::vector<std::pair<std::string, float>>>&&
             conditions,
         std::string outcome_event_file, PlayerBehavior&& behavior_diff,
         std::string battle_file_path = "");

  void Do();
  bool CheckCondition();
  std::string GetDescription() const;

  const std::vector<std::string>& GetPossibleWords() const;

  static bool CheckThisCondition(std::string type, std::string condition,
                                 float value = 0);
  static Action* CreateActionFromFile(std::string file);

 private:
  std::map<std::string, std::vector<std::pair<std::string, float>>> conditions;
  std::string outcome_event_file;
  PlayerBehavior behavior_diff;
  std::string description;
  std::string battle_file_path;
  std::vector<std::string> possible_words_;
};
#endif  // !ACTION

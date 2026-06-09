#ifndef SMALL_EVENT
#define SMALL_EVENT
#include <string>
#include <vector>

#include "action.h"

class SmallEvent {
 public:
  SmallEvent(std::string description, std::string background,
             std::vector<Action*>&& actions);

  void ShowEvent();

  static SmallEvent* GetSmallEventFromFile(std::string file_path);

  ~SmallEvent();

  const std::vector<Action*>& GetAllPossibleActions() const;

 private:
  void PrintDiscriptionToConsole();
  void PrintPossibleActions();

  std::vector<Action*> possible_actions_;
  std::string description_;
  std::string background_name_;
};
#endif  // !SMALL_EVENT

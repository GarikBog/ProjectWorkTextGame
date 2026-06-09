#ifndef BIG_EVENT
#define BIG_EVENT
#include "small_event.h"

class BigEvent {
 public:
  BigEvent(std::string first_event_path);
  void Start();
  void Stop();
  void LoadSmallEventFromFile(std::string file_path);
  void ChooseAction(int number);
  void Update();
  void PlayerWriteWord(std::string word);

  ~BigEvent();

 private:
  SmallEvent* current_event_;
  std::string need_load_event_;
  bool is_on_pause_ = true;
};

#endif  // !BIG_EVENT

#include "big_event.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "../Basic/game_state.h"

namespace {

std::string ToLowercase(const std::string& str) {
  std::string result = str;
  std::transform(result.begin(), result.end(), result.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  return result;
}

int ComputeLevenshteinDistance(const std::string& s1, const std::string& s2) {
  int m = static_cast<int>(s1.length());
  int n = static_cast<int>(s2.length());

  std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));

  for (int i = 0; i <= m; ++i) {
    dp[i][0] = i;
  }
  for (int j = 0; j <= n; ++j) {
    dp[0][j] = j;
  }

  for (int i = 1; i <= m; ++i) {
    for (int j = 1; j <= n; ++j) {
      int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
      int min_val = dp[i - 1][j] + 1;
      if (dp[i][j - 1] + 1 < min_val) min_val = dp[i][j - 1] + 1;
      if (dp[i - 1][j - 1] + cost < min_val) min_val = dp[i - 1][j - 1] + cost;
      dp[i][j] = min_val;
    }
  }

  return dp[m][n];
}

}  // namespace

void BigEvent::PlayerWriteWord(std::string word) {
  if (is_on_pause_) return;
  static constexpr int kMaxAllowedDistance = 2;

  std::string lowercase_word = ToLowercase(word);

  if (current_event_ == nullptr) {
    return;
  }

  const std::vector<Action*>& actions = current_event_->GetAllPossibleActions();

  Action* best_action = nullptr;
  int best_distance = (std::numeric_limits<int>::max)();
  std::string best_matched_word;

  for (Action* action : actions) {
    if (action == nullptr) {
      continue;
    }

    const std::vector<std::string>& possible_words = action->GetPossibleWords();

    for (const std::string& candidate : possible_words) {
      std::string lowercase_candidate = ToLowercase(candidate);

      if (lowercase_word.find(lowercase_candidate) != std::string::npos) {
        best_action = action;
        best_matched_word = candidate;
        best_distance = 0;
        break;
      }

      int length_diff =
          std::abs(static_cast<int>(lowercase_word.length()) -
                   static_cast<int>(lowercase_candidate.length()));
      if (length_diff > kMaxAllowedDistance) {
        continue;
      }

      int distance =
          ComputeLevenshteinDistance(lowercase_word, lowercase_candidate);

      if (distance < best_distance) {
        best_distance = distance;
        best_action = action;
        best_matched_word = candidate;
      }
    }

    if (best_distance == 0) {
      break;
    }
  }

  if (best_action != nullptr && best_distance <= kMaxAllowedDistance) {
    if (best_distance > 0) {
      // Сообщить игроку: "Вы имели в виду '" + best_matched_word + "'?"
    }
    best_action->Do();
  } else {
    // Сообщить игроку: "Не понимаю, что вы хотите сделать. Попробуйте другую
    // формулировку."
  }
}

BigEvent::~BigEvent() { delete current_event_; }

BigEvent::BigEvent(std::string first_event_path)
    : need_load_event_(first_event_path), current_event_(nullptr) {}

void BigEvent::Start() { is_on_pause_ = false; }

void BigEvent::Stop() { is_on_pause_ = true; }

void BigEvent::LoadSmallEventFromFile(std::string file_path) {
  need_load_event_ = file_path;
}

void BigEvent::ChooseAction(int number) {}

void BigEvent::Update() {
  if (is_on_pause_) return;
  if (!need_load_event_.empty()) {
    if (need_load_event_ == "END") {
      GameState::GetGameState().LoadNextBigEvent();
      return;
    }
    delete current_event_;
    current_event_ = SmallEvent::GetSmallEventFromFile(need_load_event_);
    need_load_event_ = "";
    current_event_->ShowEvent();
  }
}

#ifndef CONSOLE
#define CONSOLE




#include "widget.h"

#ifndef DEQUE
#include <deque>
#define DEQUE
#endif

#ifndef STRING
#include <string>
#define STRING
#endif

#ifndef VECTOR
#include <vector>
#define VECTOR
#endif

class Console : public Widget {
 public:
  // Constructor
  Console(sf::RenderWindow& window, const std::pair<float, float> pos = {0, 0},
          const std::pair<int, int> scale = {800, 600},
          const std::string texture_file = "",
          const sf::IntRect texture_rectangle = {0, 0, 0, 0});

  // Public methods
  void Log(const std::string& message, sf::Color color = sf::Color::White);
  void Log(const std::wstring& message, sf::Color color = sf::Color::White);
  void Update();
  void Draw() override;
  bool Click(const sf::Vector2i& mouse_pos) override;
  void HandleEvent(const sf::Event& event);

 private:
  // Font
  sf::Font font_;

  // Text storage
  std::deque<std::wstring> lines_;
  std::wstring input_text_;

  // Visual settings
  const int kFontSize_ = 32;
  const float kLineHeight_ = 34.0f;
  const float kLeftMargin_ = 75.0f;
  const float kRightMargin_ = 70.0f;
  const float kTopMargin_ = 170.0f;
  const float kBottomMargin_ = 15.0f;
  const float kInputHeight_ = 50.0f;
  const int kMaxLines_ = 1000;
  const int kMaxInputLength_ = 45;

  // Scrolling
  int scroll_offset_ = 0;
  int visible_lines_ = 0;

  // UI elements
  sf::RectangleShape input_box_;
  sf::Text input_display_;

  // Cache for optimization
  std::vector<sf::Text> text_cache_;
  bool need_rebuild_ = true;

  // Private methods
  void UpdateLayout();
  void RebuildCache();
  void AddLine(const std::wstring& text,
               const sf::Color& color = sf::Color::White);
  void ScrollUp(int lines = 1);
  void ScrollDown(int lines = 1);
  void ScrollToBottom();
  void ScrollToTop();
  void ProcessCommand(const std::wstring& cmd);

  // Helpers for string conversion
  std::wstring StringToWString(const std::string& str);
  std::string WStringToString(const std::wstring& wstr);
};

#endif 
#ifndef CONSOLE
#include "console.h"
#define CONSOLE
#endif

#ifndef WINDOWS
#include <windows.h>
#define WINDOWS
#endif

#ifndef IOSTREAM
#include <iostream>
#define IOSTREAM
#endif

std::wstring Console::StringToWString(const std::string& str) {
  if (str.empty()) return L"";

  int size = MultiByteToWideChar(CP_ACP, 0, str.c_str(),
                                 static_cast<int>(str.size()), NULL, 0);
  std::wstring result(size, 0);
  MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.size()),
                      &result[0], size);
  return result;
}

std::string Console::WStringToString(const std::wstring& wstr) {
  if (wstr.empty()) return "";

  int size =
      WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(),
                          static_cast<int>(wstr.size()), NULL, 0, NULL, NULL);
  std::string result(size, 0);
  WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.size()),
                      &result[0], size, NULL, NULL);
  return result;
}

// Constructor
Console::Console(sf::RenderWindow& window, const std::pair<float, float> pos,
                 const std::pair<int, int> scale,
                 const std::string texture_file,
                 const sf::IntRect texture_rectangle)
    : Widget(window, pos, scale, texture_file, texture_rectangle) {
  // Load font
  if (!font_.loadFromFile("../fonts/DwarvenStonecraftCyr.otf")) {
    std::cerr << "ERROR: Failed to load font!" << std::endl;
  }

  // Setup input box
  input_box_.setFillColor(sf::Color(20, 20, 20, 230));
  input_box_.setOutlineThickness(1);
  input_box_.setOutlineColor(sf::Color(150, 150, 150));

  // Setup input display
  input_display_.setFont(font_);
  input_display_.setCharacterSize(kFontSize_);
  input_display_.setFillColor(sf::Color::White);

  // Initial messages
  Log("Текст");
  UpdateLayout();
  ScrollToBottom();
}

// Public methods
void Console::Log(const std::string& message, sf::Color color) {
  AddLine(StringToWString(message), color);
}

void Console::Log(const std::wstring& message, sf::Color color) {
  AddLine(message, color);
}

void Console::Update() {
  // Update input text position
  float x = GetX() + kLeftMargin_;
  float y = GetY() + GetHeight() - kInputHeight_ - kBottomMargin_;

  input_display_.setString(input_text_);
  input_display_.setPosition(x + 2, y + 5);

  // Rebuild cache if needed
  if (need_rebuild_) {
    RebuildCache();
  }
}

void Console::Draw() {
  // Draw background sprite from Widget
  Widget::Draw();

  // Draw text lines
  for (auto& text : text_cache_) {
    window_.draw(text);
  }

  // Draw input area
  window_.draw(input_box_);
  window_.draw(input_display_);
}

bool Console::Click(const sf::Vector2i& mouse_pos) {
  return CheckMouseBorders(mouse_pos);
}

void Console::HandleEvent(const sf::Event& event) {
  if (event.type == sf::Event::TextEntered) {
    // Regular characters
    if (event.text.unicode >= 32 && event.text.unicode != 127) {
      if (static_cast<int>(input_text_.length()) < kMaxInputLength_) {
        input_text_ += static_cast<wchar_t>(event.text.unicode);
      }
    }
    // Backspace
    else if (event.text.unicode == 8) {
      if (!input_text_.empty()) {
        input_text_.pop_back();
      }
    }
    // Enter
    else if (event.text.unicode == 13) {
      ProcessCommand(input_text_);
      input_text_.clear();
      ScrollToBottom();
    }
  } else if (event.type == sf::Event::KeyPressed) {
    // Page Up
    if (event.key.code == sf::Keyboard::PageUp) {
      ScrollUp(visible_lines_);
    }
    // Page Down
    else if (event.key.code == sf::Keyboard::PageDown) {
      ScrollDown(visible_lines_);
    }
    // Home
    else if (event.key.code == sf::Keyboard::Home) {
      ScrollToTop();
    }
    // End
    else if (event.key.code == sf::Keyboard::End) {
      ScrollToBottom();
    }
  } else if (event.type == sf::Event::MouseWheelScrolled) {
    if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
      if (event.mouseWheelScroll.delta > 0) {
        ScrollUp(3);
      } else {
        ScrollDown(3);
      }
    }
  }
}

// Private methods
void Console::UpdateLayout() {
  float width = static_cast<float>(GetWidth());
  float height = static_cast<float>(GetHeight());

  input_box_.setSize(
      sf::Vector2f(width - kLeftMargin_ - kRightMargin_, kInputHeight_));
  input_box_.setPosition(GetX() + kLeftMargin_,
                         GetY() + height - kInputHeight_ - kBottomMargin_);

  visible_lines_ = static_cast<int>(
      (height - kInputHeight_ - kTopMargin_ - kBottomMargin_) / kLineHeight_);
  need_rebuild_ = true;
}

void Console::RebuildCache() {
  text_cache_.clear();

  float y = GetY() + kTopMargin_;
  int start = scroll_offset_;
  int end = start + visible_lines_;
  if (end > static_cast<int>(lines_.size())) {
    end = static_cast<int>(lines_.size());
  }

  for (int i = start; i < end; i++) {
    sf::Text text;
    text.setFont(font_);
    text.setString(lines_[i]);
    text.setCharacterSize(kFontSize_);
    text.setFillColor(sf::Color::White);
    text.setPosition(GetX() + kLeftMargin_, y);

    // Color for commands
    if (lines_[i].find(L"> ") == 0) {
      text.setFillColor(sf::Color::Green);
    }

    text_cache_.push_back(text);
    y += kLineHeight_;
  }

  need_rebuild_ = false;
}

void Console::AddLine(const std::wstring& text, const sf::Color& color) {
  lines_.push_back(text);

  if (static_cast<int>(lines_.size()) > kMaxLines_) {
    lines_.pop_front();
    if (scroll_offset_ > 0) {
      scroll_offset_--;
    }
  }

  // Auto-scroll to bottom if user was at bottom
  if (scroll_offset_ + visible_lines_ >= static_cast<int>(lines_.size()) - 1) {
    ScrollToBottom();
  }

  need_rebuild_ = true;
}

void Console::ScrollUp(int lines) {
  scroll_offset_ -= lines;
  if (scroll_offset_ < 0) {
    scroll_offset_ = 0;
  }
  need_rebuild_ = true;
}

void Console::ScrollDown(int lines) {
  int max_scroll = static_cast<int>(lines_.size()) - visible_lines_;
  if (max_scroll < 0) {
    max_scroll = 0;
  }

  scroll_offset_ += lines;
  if (scroll_offset_ > max_scroll) {
    scroll_offset_ = max_scroll;
  }
  need_rebuild_ = true;
}

void Console::ScrollToBottom() {
  scroll_offset_ = static_cast<int>(lines_.size()) - visible_lines_;
  if (scroll_offset_ < 0) {
    scroll_offset_ = 0;
  }
  need_rebuild_ = true;
}

void Console::ScrollToTop() {
  scroll_offset_ = 0;
  need_rebuild_ = true;
}

void Console::ProcessCommand(const std::wstring& cmd) {
  if (cmd.empty()) return;

  AddLine(L"You: " + cmd, sf::Color::Green);
}

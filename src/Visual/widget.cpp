#include "widget.h"

#include <iostream>

#include "../Basic/game_state.h"

void Widget::SetPos(const std::pair<float, float> pos) {
  x_ = pos.first;
  y_ = pos.second;
  sprite_.setPosition({x_, y_});
  if (use_background_) {
    background_.setPosition(x_, y_);
  }
}

void Widget::SetPos(const float x, const float y) {
  x_ = x;
  y_ = y;
  sprite_.setPosition({x, y});
  if (use_background_) {
    background_.setPosition(x_, y_);
  }
}

void Widget::SetX(const float x) {
  x_ = x;
  sprite_.setPosition(x_, y_);
  if (use_background_) {
    background_.setPosition(x_, y_);
  }
}

void Widget::SetY(const float y) {
  y_ = y;
  sprite_.setPosition({x_, y_});
  if (use_background_) {
    background_.setPosition(x_, y_);
  }
}

void Widget::SetScale(const std::pair<int, int> size) {
  width_ = size.first;
  height_ = size.second;
  UpdateScale();
  if (use_background_) {
    background_.setSize(sf::Vector2f(width_, height_));
    background_.setPosition(x_, y_);
  }
}

void Widget::SetScale(const int width, int height) {
  width_ = width;
  height_ = height;
  UpdateScale();
  if (use_background_) {
    background_.setSize(sf::Vector2f(width_, height_));
    background_.setPosition(x_, y_);
  }
}

void Widget::SetWidth(const int width) {
  width_ = width;
  if (use_background_) {
    background_.setSize(sf::Vector2f(width_, height_));
  }
}

void Widget::SetHeight(const int height) {
  height_ = height;
  if (use_background_) {
    background_.setSize(sf::Vector2f(width_, height_));
  }
}

void Widget::SetIcon(Icon* icon) {
  if (!icon) return;

  icon_ = icon;

  sprite_.setTexture(icon_->GetTexture());
  sprite_.setTextureRect(icon_->GetTextureRect());
}

void Widget::SetTexture(const std::string texture_file) {
  icon_->SetTexture(texture_file);
}

void Widget::SetTextureRect(const sf::IntRect rect) {
  icon_->SetTextureRect(rect);
}

std::pair<float, float> Widget::GetPos() const { return {x_, y_}; }

float Widget::GetX() const { return x_; }

float Widget::GetY() const { return y_; }

sf::FloatRect Widget::GetCollision() const { return sprite_.getGlobalBounds(); }

std::pair<int, int> Widget::GetScale() const { return {width_, height_}; }

int Widget::GetWidth() const { return width_; }

int Widget::GetHeight() const { return height_; }

void Widget::Draw() {
  if (use_background_) {
    window_.draw(background_);
  }
  if (is_visible_) {
    window_.draw(sprite_);
  }
}

void Widget::SetVisibility(bool is_visible_now) {
  is_visible_ = is_visible_now;
}

Widget::Widget(sf::RenderWindow& window, const std::pair<float, float> pos,
               const std::pair<int, int> scale, const std::string texture_file,
               const sf::IntRect texture_rectangle)
    : window_(window) {
  self_made_icon_ = true;
  SetIcon(new Icon(texture_file, texture_rectangle));
  SetPos(pos);
  SetScale(scale);
  SetVisibility(true);
}

Widget::Widget(sf::RenderWindow& window, Icon* icon,
               const std::pair<float, float> pos,
               const std::pair<int, int> scale)
    : window_(window) {
  SetIcon(icon);
  SetPos(pos);
  SetScale(scale);
  SetVisibility(true);
}

Widget::~Widget() {
  if (self_made_icon_) {
    delete icon_;
  }
}

void Widget::UpdateScale() {
  sprite_.setScale({width_ / sprite_.getLocalBounds().width,
                    height_ / sprite_.getLocalBounds().height});
}

bool Widget::CheckMouseBorders(const sf::Vector2i& mouse_pos) {
  sf::FloatRect bounds = GetCollision();

  return mouse_pos.x > bounds.left &&
         mouse_pos.x < bounds.left + bounds.width && mouse_pos.y > bounds.top &&
         mouse_pos.y < bounds.top + bounds.height;
}

void Widget::SetBackgroundColor(const sf::Color& color) {
  if (!use_background_) {
    background_.setSize(sf::Vector2f(width_, height_));
    background_.setPosition(x_, y_);
    use_background_ = true;
  }
  background_.setFillColor(color);
}

void Widget::ClearBackgroundColor() { use_background_ = false; }

void Widget::SetOutlineColor(const sf::Color& color) {
  if (!use_background_) {
    background_.setSize(sf::Vector2f(width_, height_));
    background_.setPosition(x_, y_);
    use_background_ = true;
  }
  background_.setOutlineColor(color);
  background_.setOutlineThickness(2.0f);
}

void Widget::SetOutlineThickness(float thickness) {
  if (use_background_) {
    background_.setOutlineThickness(thickness);
  }
}

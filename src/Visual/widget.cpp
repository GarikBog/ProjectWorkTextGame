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

  if (self_made_icon_) delete icon_;
  icon_ = icon;

  sprite_.setTexture(icon_->GetTexture());
  sprite_.setTextureRect(icon_->GetTextureRect());

  SetScale(width_, height_);
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

// Вспомогательный метод для копирования состояния
void Widget::CopyFrom(const Widget& other) {
  x_ = other.x_;
  y_ = other.y_;
  width_ = other.width_;
  height_ = other.height_;
  is_visible_ = other.is_visible_;
  sprite_ = other.sprite_;  // копирование спрайта (текстура shared?)
  use_background_ = other.use_background_;
  background_ = other.background_;

  // Копирование иконки
  if (other.self_made_icon_ && other.icon_) {
    // Создаём глубокую копию иконки, если она была создана самим виджетом
    icon_ = new Icon(*other.icon_);  // требуется конструктор копирования в Icon
    self_made_icon_ = true;
  } else {
    // Иконка принадлежит внешнему владельцу – просто копируем указатель
    icon_ = other.icon_;
    self_made_icon_ = false;
  }

  // Обновляем спрайт, если иконка изменилась
  if (icon_) {
    sprite_.setTexture(icon_->GetTexture());
    sprite_.setTextureRect(icon_->GetTextureRect());
  }
}

// Конструктор копирования
Widget::Widget(const Widget& other)
    : window_(
          other.window_)  // инициализация ссылки (привязываемся к тому же окну)
{
  CopyFrom(other);
}

// Оператор присваивания
Widget& Widget::operator=(const Widget& other) {
  if (this == &other) return *this;

  // Освобождаем ресурсы текущего объекта
  if (self_made_icon_) {
    delete icon_;
    icon_ = nullptr;
    self_made_icon_ = false;
  }

  // Копируем состояние (кроме ссылки window_, которая уже привязана)
  CopyFrom(other);

  return *this;
}

// widget.cpp

// Конструктор перемещения
Widget::Widget(Widget&& other) noexcept
    : window_(other.window_)  // ссылка – привязываемся к тому же окну
      ,
      x_(std::exchange(other.x_, 0)),
      y_(std::exchange(other.y_, 0)),
      width_(std::exchange(other.width_, 0)),
      height_(std::exchange(other.height_, 0)),
      self_made_icon_(std::exchange(other.self_made_icon_, false)),
      icon_(std::exchange(other.icon_, nullptr)),
      sprite_(std::move(other.sprite_)),
      background_(std::move(other.background_)),
      use_background_(std::exchange(other.use_background_, false)),
      is_visible_(std::exchange(other.is_visible_, false)) {
  // После перемещения other больше не владеет ресурсами
  // и не должен их удалять в деструкторе.
}

// Оператор присваивания с перемещением
Widget& Widget::operator=(Widget&& other) noexcept {
  if (this == &other) return *this;

  // Освобождаем текущие ресурсы
  if (self_made_icon_) {
    delete icon_;
  }

  // Перемещаем данные
  x_ = std::exchange(other.x_, 0);
  y_ = std::exchange(other.y_, 0);
  width_ = std::exchange(other.width_, 0);
  height_ = std::exchange(other.height_, 0);
  self_made_icon_ = std::exchange(other.self_made_icon_, false);
  icon_ = std::exchange(other.icon_, nullptr);
  sprite_ = std::move(other.sprite_);
  background_ = std::move(other.background_);
  use_background_ = std::exchange(other.use_background_, false);
  is_visible_ = std::exchange(other.is_visible_, false);

  // Ссылка window_ уже привязана к текущему объекту – ничего не делаем

  return *this;
}

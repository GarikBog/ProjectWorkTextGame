#ifndef WIDGET

#define WIDGET

#include "icon.h"

class Widget {
 public:
  void SetPos(const std::pair<float, float> pos);
  void SetPos(const float x, const float y);
  void SetX(const float x);
  void SetY(const float y);

  void SetScale(const std::pair<int, int> size);
  void SetScale(const int width, int height);
  void SetWidth(const int width);
  void SetHeight(const int height);

  void SetIcon(Icon* icon);
  void SetTexture(const std::string texture_file);
  void SetTextureRect(const sf::IntRect rect);

  // Getters
  std::pair<float, float> GetPos() const;
  float GetX() const;
  float GetY() const;
  sf::FloatRect GetCollision() const;

  std::pair<int, int> GetScale() const;
  int GetWidth() const;
  int GetHeight() const;

  // Tech & other
  virtual void Draw();
  virtual void SetVisibility(bool is_visible_now);

  Widget(sf::RenderWindow& window, const std::pair<float, float> pos = {0, 0},
         const std::pair<int, int> scale = {0, 0},
         const std::string texture_file = "",
         const sf::IntRect texture_rectangle = {0, 0, 0, 0});
  Widget(sf::RenderWindow& window, Icon* icon = nullptr,
         const std::pair<float, float> pos = {0, 0},
         const std::pair<int, int> scale = {0, 0});
  virtual ~Widget();

  virtual void SetBackgroundColor(const sf::Color& color);
  virtual void ClearBackgroundColor();
  virtual void SetOutlineColor(const sf::Color& color);
  virtual void SetOutlineThickness(float thickness);

 protected:
  // position
  float x_ = 0, y_ = 0;

  // size
  int width_ = 0, height_ = 0;

  bool self_made_icon_ = false;
  bool is_visible_ = false;

  Icon* icon_ = nullptr;

  sf::Sprite sprite_;

  sf::RenderWindow& window_;

  sf::RectangleShape background_;
  bool use_background_ = false;

  void UpdateScale();
  bool CheckMouseBorders(const sf::Vector2i& mouse_pos);
};

#endif

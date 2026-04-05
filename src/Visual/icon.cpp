#ifndef ICON
#include "icon.h"
#define ICON
#endif  // !ICON

#include "../Basic/game_state.h"
void Icon::SetTexture(const std::string texture_file) {
  if (!texture_.loadFromFile(texture_file) &&
      !texture_.loadFromFile(ASSETS_PATH + texture_file)) {
    texture_.loadFromFile(ASSETS_PATH "/textures/error.jpg");
    texture_rectangle_ = {0, 0, 800, 800};
  }
}

void Icon::SetTextureRect(const sf::IntRect rect) { texture_rectangle_ = rect; }

const sf::Texture& Icon::GetTexture() const { return texture_; }

const sf::IntRect& Icon::GetTextureRect() const { return texture_rectangle_; }

Icon::Icon(const std::string texture_file, const sf::IntRect rect) {
  SetTexture(texture_file);
  SetTextureRect(rect);
}

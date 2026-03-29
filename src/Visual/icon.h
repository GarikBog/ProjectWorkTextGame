#ifndef ICON
#define ICON



#ifndef SFML_GRAPHICS
#define SFML_GRAPHICS
#include <SFML/Graphics.hpp>
#endif // !

class Icon {
 public:
  void SetTexture(const std::string texture_file);
  void SetTextureRect(const sf::IntRect rect);

  const sf::Texture& GetTexture() const;
  const sf::IntRect& GetTextureRect() const;

  Icon(const std::string texture_file = "",
       const sf::IntRect rect = {0, 0, 0, 0});

 protected:
 private:
  sf::Texture texture_;

  sf::IntRect texture_rectangle_;
};


#endif
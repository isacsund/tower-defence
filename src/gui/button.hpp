#pragma once
#include <SFML/Graphics.hpp>

class Button : public sf::Drawable {
 public:
  Button(std::string title, sf::Font font, sf::Vector2f position);

  sf::Vector2u GetSize();

 private:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

  sf::Sprite sprite_;
  sf::Text title_;
  sf::Font font_;
  sf::Vector2f position_;
};
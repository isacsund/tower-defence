#include "button.hpp"
#include <iostream>
#include "../game/texturemanager.hpp"

Button::Button(std::string title, sf::Font font, sf::Vector2f position)
    : font_(font), position_(position) {
  // title_.setFont(font_);
  title_.setString(title);
  title_.setCharacterSize(30);
  title_.setStyle(sf::Text::Regular);
  title_.setPosition(sf::Vector2f(10.0, 10.0));
  title_.setFillColor(sf::Color::White);
  sprite_.setTexture(texture_manager.GetTexture("sprites/button.png"));
  sprite_.setPosition(position_);
}

sf::Vector2u Button::GetSize() { return sprite_.getTexture()->getSize(); }

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(sprite_, states);
  target.draw(title_, states);
}
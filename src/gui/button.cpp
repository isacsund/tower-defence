#include "button.hpp"
#include <iostream>
#include "../game/texturemanager.hpp"

Button::Button(std::string title, sf::Font& font, sf::Vector2f position)
    : font_(font), position_(position) {
  title_.setFont(font);
  title_.setString(title);
  title_.setCharacterSize(30);
  title_.setStyle(sf::Text::Regular);
  title_.setPosition(position);
  title_.setFillColor(sf::Color::White);
  sprite_.setTexture(texture_manager.GetTexture("sprites/button.png"));
  sprite_.setPosition(position_);
}

sf::Vector2u Button::GetSize() { return sprite_.getTexture()->getSize(); }

void Button::SetPosition(sf::Vector2f position) {
  position_ = position;
  sprite_.setPosition(position_);
  title_.setPosition(position_);
};

bool Button::Contains(sf::Vector2f mouse_position) {
  return sprite_.getGlobalBounds().contains(mouse_position);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(sprite_, states);
  target.draw(title_, states);
}
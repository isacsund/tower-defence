#include "button.hpp"
#include <iostream>
#include "../game/texturemanager.hpp"

Button::Button(std::string title, sf::Font& font, sf::Vector2f position,
               const std::string& texture_name)
    : font_(font), position_(position) {
  title_.setFont(font);
  title_.setString(title);
  title_.setCharacterSize(30);
  title_.setStyle(sf::Text::Regular);

  title_.setFillColor(sf::Color::White);
  sprite_.setTexture(texture_manager.GetTexture(texture_name));
  SetPosition(position_);
}

sf::Vector2u Button::GetSize() { return sprite_.getTexture()->getSize(); }

void Button::SetPosition(sf::Vector2f position) {
  position_ = position;
  auto text_size = title_.getLocalBounds();
  auto button_size = sprite_.getLocalBounds();
  title_.setPosition(
      sf::Vector2f(position_.x + button_size.width / 2 - text_size.width / 2,
                   position_.y + button_size.height / 2 - text_size.height));
  sprite_.setPosition(position_);
}

bool Button::Contains(sf::Vector2f mouse_position) {
  return sprite_.getGlobalBounds().contains(mouse_position);
}

const sf::Vector2f Button::GetPosition() const { return position_; };
void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(sprite_, states);
  target.draw(title_, states);
}
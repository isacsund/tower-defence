#include "guientry.hpp"
#include "../game/texturemanager.hpp"

GuiEntry::GuiEntry(sf::Vector2f position, boost::optional<std::string> title,
                   boost::optional<std::string> texture_name,
                   boost::optional<sf::Font&> font)
    : position_(position) {
  if (title.get_ptr() != 0) {
    title_ = sf::Text();
    title_->setFont(font.get());
    title_->setString(title.get());
    title_->setCharacterSize(30);
    title_->setStyle(sf::Text::Regular);
    title_->setFillColor(sf::Color::White);
  }
  if (texture_name.get_ptr() != 0) {
    sprite_ = sf::Sprite();
    sprite_->setTexture(texture_manager.GetTexture(texture_name.get()));
  }
  SetPosition(position_);
}

void GuiEntry::SetPosition(sf::Vector2f position) {
  position_ = position;
  if (title_.get_ptr() != 0) {
    auto text_size = title_->getLocalBounds();
    if (sprite_.get_ptr() != 0) {
      auto button_size = sprite_->getLocalBounds();
      title_->setPosition(sf::Vector2f(
          position_.x + button_size.width / 2 - text_size.width / 2,
          position_.y + button_size.height / 2 - text_size.height));
    } else {
      title_->setPosition(sf::Vector2f(position_.x - text_size.width / 2,
                                       position_.y - text_size.height));
    }
  }
  if (sprite_.get_ptr() != 0) {
    sprite_->setPosition(position_);
  }
}

bool GuiEntry::Contains(sf::Vector2f mouse_position) {
  return sprite_->getGlobalBounds().contains(mouse_position);
}

void GuiEntry::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  if (sprite_.get_ptr() != 0) target.draw(sprite_.get(), states);
  if (title_.get_ptr() != 0) target.draw(title_.get(), states);
}
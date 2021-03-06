#include "guientry.hpp"
#include <iostream>
#include "../game/texturemanager.hpp"

GuiEntry::GuiEntry(sf::Vector2f position, boost::optional<std::string> title,
                   boost::optional<sf::Texture&> texture,
                   boost::optional<sf::Font&> font, bool visible)
    : position_(position),
      visible_(visible),
      enabled_(true),
      highlighted_(false) {
  if (title.get_ptr() != 0) {
    title_ = sf::Text();
    title_->setFont(font.get());
    title_->setString(title.get());
    title_->setCharacterSize(20);
    title_->setStyle(sf::Text::Regular);
    title_->setFillColor(sf::Color::White);
  }
  if (texture.get_ptr() != 0) {
    sprite_ = sf::Sprite();
    sprite_->setTexture(texture.get());
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
      title_->setPosition(sf::Vector2f(position_.x, position_.y));
    }
  }
  if (sprite_.get_ptr() != 0) {
    sprite_->setPosition(position_);
  }
}

sf::Vector2f GuiEntry::GetPosition() const { return position_; }

void GuiEntry::SetTitle(const std::string& title) { title_->setString(title); }
std::string GuiEntry::GetTitle() const {
  return title_->getString().toAnsiString();
}

float GuiEntry::GetWidth() const {
  if (sprite_.get_ptr() != 0) {
    return sprite_->getLocalBounds().width;
  } else {
    return title_->getLocalBounds().width;
  }
}

float GuiEntry::GetHeight() const {
  if (sprite_.get_ptr() != 0) {
    return sprite_->getLocalBounds().height;
  } else {
    return title_->getLocalBounds().height;
  }
}

bool GuiEntry::Contains(sf::Vector2f mouse_position) {
  return sprite_->getGlobalBounds().contains(mouse_position);
}

void GuiEntry::Show() { visible_ = true; }
void GuiEntry::Hide() { visible_ = false; }
bool GuiEntry::IsVisible() { return visible_; }

void GuiEntry::Enable() {
  enabled_ = true;
  sprite_->setColor(sf::Color(sf::Color::White));
}
void GuiEntry::Disable() {
  enabled_ = false;
  sprite_->setColor(sf::Color(128, 128, 128, 255));
}
bool GuiEntry::IsEnabled() { return enabled_; }

void GuiEntry::Highlight() {
  highlighted_ = true;
  sprite_->setColor(sf::Color(192, 192, 192, 255));
}
void GuiEntry::Unhighlight() {
  highlighted_ = false;
  sprite_->setColor(sf::Color(sf::Color::White));
}

void GuiEntry::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  if (visible_) {
    if (sprite_.get_ptr() != 0) target.draw(sprite_.get(), states);
    if (title_.get_ptr() != 0) target.draw(title_.get(), states);
  }
}

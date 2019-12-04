#include "gui.hpp"

Gui::Gui() { visible_ = true; }
void Gui::Show() { visible_ = true; }
void Gui::Hide() { visible_ = false; }

void Gui::Add(const std::string& name, GuiEntry entry) {
  entries_.insert({name, entry});
}

GuiEntry& Gui::Get(const std::string& name) { return entries_.at(name); }

void Gui::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  if (!visible_) return;
  for (auto entry : entries_) {
    target.draw(entry.second, states);
  }
}
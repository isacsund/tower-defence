#pragma once
#include <SFML/Graphics.hpp>
#include "guientry.hpp"

class Gui : public sf::Drawable {
 public:
  Gui();
  void Show();
  void Hide();
  void Add(const std::string& name, GuiEntry entry);
  GuiEntry& Get(const std::string& name);
  bool Has(const std::string& name);

 private:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  std::map<std::string, GuiEntry> entries_;
  bool visible_;
};

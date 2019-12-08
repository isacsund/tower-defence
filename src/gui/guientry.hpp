#include <SFML/Graphics.hpp>
#include <boost/optional.hpp>

class GuiEntry : public sf::Drawable {
 public:
  GuiEntry(sf::Vector2f position, boost::optional<std::string> title,
           boost::optional<sf::Texture&> texture,
           boost::optional<sf::Font&> font, bool visible = true);
  void SetPosition(sf::Vector2f position);
  void SetTitle(const std::string& title);
  std::string GetTitle() const;
  float GetWidth() const;
  float GetHeight() const;
  bool Contains(sf::Vector2f mouse_position);
  void Show();
  void Hide();
  bool IsVisible();
  void Enable();
  void Disable();
  bool IsEnabled();
  void Highlight();
  void Unhighlight();

 private:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  boost::optional<sf::Text> title_;
  boost::optional<sf::Sprite> sprite_;
  sf::Vector2f position_;
  bool visible_;
  bool enabled_;
  bool highlighted_;
};
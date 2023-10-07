#ifndef TEXT_HPP
#define TEXT_HPP

#include <SFML/Graphics.hpp>
#include <vector>

namespace pf {

class Text {
 private:
  sf::Font font_;
  Epidemic epidemic_;
  sf::RenderWindow window_;

 public:
    //Constructor
  Text(Epidemic, sf::RenderWindow);

  //Graph's title
  sf::Text set_Title();

  // titolo asse x
  sf::Text x_axis_name();

  // titolo asse y
  sf::Text y_axis_name();

  // x numbers
  std::vector<sf::Text> x_numbers();

  // y numbers
  std::vector<sf::Text> y_numbers();
};
}  // namespace pf
#endif

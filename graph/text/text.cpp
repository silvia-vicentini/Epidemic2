
#include "text.hpp"

namespace pf {

Text::Text(Epidemic epidemic, sf::RenderWindow window) : epidemic_(epidemic), window_(window) {
  font_.loadFromFile("Arialn.ttf");
}

//set graph title
sf::Text Text::set_Title() {
  sf::Text Title("Epidemic Evolution", font_, 25);
  Title.setFillColor(sf::Color::Black);
  Title.setPosition((window_.getSize().x - 160.f) / 2, 30.f);
  return Title;
}

//set x title
sf::Text Text::x_axis_name() {
  sf::Text xAxisName("Days", font_, 20);
  xAxisName.setFillColor(sf::Color::Black);
  xAxisName.setPosition(window_.getSize().x - 150, window_.getSize().y - 40);
  return xAxisName;
}

//set y title
sf::Text Text::y_axis_name() {
  sf::Text yAxisName("Number of people", font_, 20);
  yAxisName.setFillColor(sf::Color::Black);
  yAxisName.setPosition(15, 230);
  yAxisName.setRotation(-90.f);
  return yAxisName;
}

//set x numbers
std::vector<sf::Text> Text::x_numbers() {
  std::vector<sf::Text> days;
  long int T = epidemic_.getT();
  for (int i = 0; i <= 8; ++i) {
    std::string xstring = std::to_string(T * i / 8);
    sf::Text xnumber(xstring, font_, 20);
    xnumber.setFillColor(sf::Color::Black);
    xnumber.setPosition(80.f + (window_.getSize().x - 160.f) * i / 8, window_.getSize().y - 70);
    days.push_back(xnumber);
  }
  return days;
}

//set y numbers
std::vector<sf::Text> Text::y_numbers() {
  std::vector<sf::Text> people;
  long int N = epidemic_.N();
  for (int i = 0; i <= 6; ++i) {
    std::string ystring = std::to_string(N * i / 6);
    sf::Text ynumber(ystring, font_, 20);
    ynumber.setFillColor(sf::Color::Black);
    ynumber.setPosition(50.f, (window_.getSize().y - 90.f) - (window_.getSize().y - 2 * 80.f) * i / 6);
    people.push_back(ynumber);
  }
  return people;
}
}  // namespace pf

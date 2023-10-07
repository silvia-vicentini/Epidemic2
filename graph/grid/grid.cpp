#include "grid.hpp"

namespace pf {
Grid::Grid(sf::RenderWindow window) : window_(window){}

sf::VertexArray Grid::bottom_side() {
  sf::VertexArray BottomSide(sf::Lines, 2);
  BottomSide[0].position = sf::Vector2f(80.f, window_.getSize().y  - 80.f);
  BottomSide[1].position = sf::Vector2f(window_.getSize().x - 80.f, window_.getSize().y - 80.f);
  BottomSide[0].color = sf::Color::Black;
  BottomSide[1].color = sf::Color::Black;
  return BottomSide;
}

sf::VertexArray Grid::top_side() {
  sf::VertexArray TopSide(sf::Lines, 2);
  TopSide[0].position = sf::Vector2f(80.f, window_.getSize().y - 80.f);
  TopSide[1].position = sf::Vector2f(80.f, 80.f);
  TopSide[0].color = sf::Color::Black;
  TopSide[1].color = sf::Color::Black;
  return TopSide;
}

sf::VertexArray Grid::left_side() {
  sf::VertexArray grid::LeftSide(sf::Lines, 2);
  LeftSide[0].position = sf::Vector2f(80.f, 80.f);
  LeftSide[1].position = sf::Vector2f(window_.getSize().x - 80.f, 80.f);
  LeftSide[0].color = sf::Color::Black;
  LeftSide[1].color = sf::Color::Black;
  return LeftSide;
}

sf::VertexArray Grid::right_side() {
  sf::VertexArray RightSide(sf::Lines, 2);
  RightSide[0].position = sf::Vector2f(window_.getSize().x - 80.f, window_.getSize().y - 80.f);
  RightSide[1].position = sf::Vector2f(window_.getSize().x - 80.f, 80.f);
  RightSide[0].color = sf::Color::Black;
  RightSide[1].color = sf::Color::Black;
  return RightSide;
}

std::vector<sf::VertexArray> Grid::vertical_lines() {
  std::vector<sf::VertexArray> VerticalLines;
  for (int i = 1; i < xParts; ++i) {
    sf::VertexArray VerticalLine(sf::Lines, 2);
    VerticallLine[0].position =
        sf::Vector2f(80.f + (window_.getSize().x - 160.f) * i / 8, window_.getSize().y - 80.f);
    VerticallLine[1].position =
        sf::Vector2f(80.f + (window_.getSize().x - 2 * 80.f) * i / 8, 80.f);
    VerticallLine[0].color = sf::Color(175, 238, 238);
    VerticallLine[1].color = sf::Color(175, 238, 238);
    VerticalLines.push_back(VerticallLine);
  }
  return VerticalLines;
}

std::vector<sf::VertexArray> Grid::horizontal_lines() {
  std::vector<sf::VertexArray> HorizontalLines;
  for (int i = 1; i < yParts; ++i) {
    sf::VertexArray HorizontalLine(sf::Lines, 2);
    HorizontalLine[0].position =
        sf::Vector2f(80.f, 80.f + (window_.getSize().y - 160.f) * i / 6);
    HorizontalLine[1].position =
        sf::Vector2f(window_.getSize().x - 80.f, 80.f + (window_.getSize().y - 160.f) * i / 6);
    HorizontalLine[0].color = sf::Color(175, 238, 238);
    HorizontalLine[1].color = sf::Color(175, 238, 238);
    HorizontalLines.push_back(HorizontalLine);
  }
  return HorizontalLines;
}
}  // namespace pf

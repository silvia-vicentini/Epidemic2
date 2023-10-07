#ifndef GRID_HPP
#define GRID_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <vector>

namespace pf {

class Grid {
 private:
    sf::RenderWindow window_;

 public:
  Grid(sf::RenderWindow);

  // lato in basso esterno della griglia
  sf::VertexArray bottom_side();

  // lato a sinistra esterno della griglia
  sf::VertexArray top_side();

  // lato in alto esterno della griglia
  sf::VertexArray left_side();

  // lato esterno a destra della griglia
  sf::VertexArray right_side();

  // righe verticali della griglia
  std::vector<sf::VertexArray> vertical_lines();

  // righe orizzontali della griglia
  std::vector<sf::VertexArray> horizontal_lines();
};
}  // namespace pf
#endif

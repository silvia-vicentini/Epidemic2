#ifndef CURVE_HPP
#define CURVE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <vector>
#include "../../functions/functions.hpp"

namespace pf {

class Curve: private Epidemic {
 private:
Epidemic epidemic_;
sf::RenderWindow window_;
/*
std::vector <Population> population_state_;
 long int T;
 long int N;
 float xscale;
float yscale;*/

 public:
  Curve(Epidemic, sf::RenderWindow);

std::vector<sf::VertexArray> S_curve();

std::vector<sf::VertexArray> I_curve() ;

std::vector<sf::VertexArray> R_curve();

std::vector<sf::CircleShape> S_dots();

std::vector<sf::CircleShape> I_dots() ;

std::vector<sf::CircleShape> R_dots();
};
}  // namespace pf
#endif            


#ifndef DRAW_HPP
#define DRAW_HPP

#include "curve/curve.hpp"
#include "grid/grid.hpp"
#include "text/text.hpp"
#include "../functions/functions.hpp"

namespace pf {

class Draw {
 private:
  sf::RenderWindow window_;
  Epidemic epidemic_;
  
  Text text;
  Grid grid;
  Curve curve;

 public:
  Draw(Epidemic);
/*
float width_;
float height_;
long int T_;
long int N_;
float xscale_;
float yscale_;
std::vector<Population> population_state_;*/
/*
float width_ = window_.getSize().x;
float height_ = window_.getSize().y;
long int T_ = epidemic_.getT();
long int N_ = epidemic_.N();
float xscale_ = (width_ - 2 * dwb_) / (T_ + 1);
float yscale_ = (height_ - 2 * dwb_) / N_;
std::vector<Population> population_state_ = epidemic_.evolve();*/

  void draw_graph();
};
}  // namespace pf
#endif

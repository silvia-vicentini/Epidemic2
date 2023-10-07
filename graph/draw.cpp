
#include "draw.hpp"

namespace pf {
/*
long int Curve::T = epidemic_.getT();
long int Curve::N = epidemic_.N();
float Curve::xscale_ = (window_.getSize().x - 160.f) / (T + 1);
float Curve::yscale_ = (window_.getSize().y - 160.f) / N;
std::vector<Population> Curve::population_state_ = epidemic_.evolve();*/

Draw::Draw(Epidemic epidemic)
    : epidemic_(epidemic), 
    window_{sf::VideoMode(1000, 600), ""}, 
    text{epidemic_, window_},
    grid{window_},
    curve{epidemic_, window_} {}
/*
Text Draw::text{epidemic_, window_};
Grid Draw::grid{window_};
Curve Draw::curve{epidemic_, window_}; */

void Draw::draw_graph() {
  while (window_.isOpen()) {
    sf::Event event;
    while (window_.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window_.close();
    }
    window_.clear(sf::Color::White);

    sf::Text Title = text.set_Title();
    window_.draw(Title);

    sf::VertexArray BottomSide = grid.bottom_side();
    window_.draw(BottomSide);

    sf::VertexArray TopSide = grid.top_side();
    window_.draw(TopSide);

    sf::VertexArray LeftSide = grid.left_side();
    window_.draw(LeftSide);

    sf::VertexArray RightSide = grid.right_side();
    window_.draw(RightSide);

    sf::Text xAxisName = text.x_axis_name();
    window_.draw(xAxisName);

    sf::Text yAxisName = text.y_axis_name();
    window_.draw(yAxisName);

    std::vector<sf::VertexArray> HorizontalLines =
        grid.horizontal_lines();
    for (const sf::VertexArray& HorizontalLine : HorizontalLines) {
      window_.draw(HorizontalLine);
    }

    std::vector<sf::VertexArray> VerticalLines =
        grid.vertical_lines();
    for (const sf::VertexArray& VerticalLine : VerticalLines) {
      window_.draw(VerticalLine);
    }

    std::vector<sf::VertexArray> Susceptible = curve.S_curve();
    for (const sf::VertexArray& SusceptibleCurve : Susceptible) {
      window_.draw(SusceptibleCurve);
    }

    std::vector<sf::VertexArray> Infected = curve.I_curve();
    for (const sf::VertexArray& InfectedCurve : Infected) {
      window_.draw(InfectedCurve);
    }

    std::vector<sf::VertexArray> Removed = curve.R_curve();
    for (const sf::VertexArray& RemovedCurve : Removed) {
      window_.draw(RemovedCurve);
    }

    std::vector<sf::CircleShape> SDots = curve.S_dots();
    for (const sf::CircleShape& Sdots : SDots) {
      window_.draw(Sdots);
    }

    std::vector<sf::CircleShape> IDots = curve.I_dots();
    for (const sf::CircleShape& Idots : IDots) {
      window_.draw(Idots);
    }

    std::vector<sf::CircleShape> RDots = curve.R_dots();
    for (const sf::CircleShape& Rdots : RDots) {
      window_.draw(Rdots);
    }

    std::vector<sf::Text> days = text.x_numbers();
    for (const sf::Text& xnumber : days) {
      window_.draw(xnumber);
    }

    std::vector<sf::Text> people = text.y_numbers();
    for (const sf::Text& ynumber : people) {
      window_.draw(ynumber);
    }

    window_.display();
  }
}
}  // namespace pf

#include "curve.hpp"

namespace pf {
  
Curve::Curve(Epidemic epidemic, sf::RenderWindow window) : epidemic_(epidemic), window_(window) {
long int T = epidemic_.getT();
float xscale_ = (window_.getSize().x - 160.f) / (T + 1);
float yscale_ = (window_.getSize().y - 160.f) / epidemic_.N();
std::vector<Population> population_state_ = epidemic_.evolve();
}

std::vector<sf::VertexArray> Curve::S_curve() {
  std::vector<sf::VertexArray> Susceptible;
  for (int i = 0; i <= T; ++i) {
    sf::VertexArray SusceptibleCurve(sf::Lines, 2);
    SusceptibleCurve[0].position = sf::Vector2f(
        80.f + xscale * i, - 80.f + window_.getSize().y - yscale * population_state_[i].S);
    SusceptibleCurve[1].position =
        sf::Vector2f(80.f + xscale * (i + 1),
                     -80.f + window_.getSize().y - yscale * population_state_[i + 1].S);
    SusceptibleCurve[0].color = sf::Color(0, 0, 255);
    SusceptibleCurve[1].color = sf::Color(0, 0, 255);
    Susceptible.push_back(SusceptibleCurve);
  }
  return Susceptible;
}

std::vector<sf::VertexArray> Curve::I_curve() {
  std::vector<sf::VertexArray> Infected;
  for (int i = 0; i <= T; ++i) {
    sf::VertexArray InfectedCurve(sf::Lines, 2);
    InfectedCurve[0].position = sf::Vector2f(
        80.f + xscale * i, -80.f + window_.getSize().y - yscale * population_state_[i].I);
    InfectedCurve[1].position =
        sf::Vector2f(80.f + xscale * (i + 1),
                     -80.f + window_.getSize().y - yscale * population_state_[i + 1].I);
    InfectedCurve[0].color = sf::Color(230, 0, 0);
    InfectedCurve[1].color = sf::Color(230, 0, 0);
    Infected.push_back(InfectedCurve);
  }
  return Infected;
}

std::vector<sf::VertexArray> Curve::R_curve() {
  std::vector<sf::VertexArray> Removed;
  for (int i = 0; i <= T; ++i) {
    sf::VertexArray RemovedCurve(sf::Lines, 2);
    RemovedCurve[0].position = sf::Vector2f(
        80.f + xscale * i, -80.f + window_.getSize().y - yscale * population_state_[i].R);
    RemovedCurve[1].position =
        sf::Vector2f(80.f + xscale * (i + 1),
                     -80.f + window_.getSize().y - yscale * population_state_[i + 1].R);
    RemovedCurve[0].color = sf::Color(20, 230, 0);
    RemovedCurve[1].color = sf::Color(20, 230, 0);
    Removed.push_back(RemovedCurve);
  }
  return Removed;
}

std::vector<sf::CircleShape> S_dots() {
  std::vector<sf::CircleShape> SDots;
  for (int i = 0; i <= T; ++i) {
    sf::CircleShape Sdots(4.f);
    Sdots.setFillColor(sf::Color(0, 0, 255));
    Sdots.setPosition(
        sf::Vector2f(80.f + xscale * (i + 1),
                     -80.f + window_.getSize().y - yscale * population_state_[i + 1].S - 2));
    SDots.push_back(Sdots);
  }
  return SDots;
}

std::vector<sf::CircleShape> I_dots() {
  std::vector<sf::CircleShape> IDots;
  for (int i = 0; i <= T; ++i) {
    sf::CircleShape Idots(4.f);
    Idots.setFillColor(sf::Color(230, 0, 0));
    Idots.setPosition(
        sf::Vector2f(80.f + xscale * (i + 1),
                     -80.f + window_.getSize().y - yscale * population_state_[i + 1].I - 2));
    IDots.push_back(Idots);
  }
  return IDots;
}

std::vector<sf::CircleShape> R_dots() {
  std::vector<sf::CircleShape> RDots;
  for (int i = 0; i <= T; ++i) {
    sf::CircleShape Rdots(4.f);
    Rdots.setFillColor(sf::Color(20, 230, 0));
    Rdots.setPosition(
        sf::Vector2f(80.f + xscale * (i + 1),
                     -80.f + window_.getSize().y - yscale * population_state_[i + 1].R - 2));
    RDots.push_back(Rdots);
  }
  return RDots;
}
}  // namespace pf

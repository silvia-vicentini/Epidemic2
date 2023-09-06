#include <vector>
#include <cmath>
#include <cassert>
#include <iostream>

struct Population {
  long int S;
  long int I;
  long int R;
};

class Epidemic {
 private:
  double m_beta;
  double m_gamma;
  Population m_initial_population;
  long int m_T;

  long int N() const{
  return m_initial_population.S + m_initial_population.I +
         m_initial_population.R;
};

  Population solve(Population prev_state) {
  Population next_state;
  next_state.S = std::round(prev_state.S - m_beta * prev_state.S *
                                               prev_state.I / N());
  next_state.I = std::round(
      prev_state.I + m_beta * prev_state.S * prev_state.I / N() -
      m_gamma * prev_state.I);
  next_state.R = std::round(prev_state.R + m_gamma * prev_state.I);
  return next_state;
};

  Population lockdown(Population prev_state) {
  Population next_state;
  next_state.S = prev_state.S;
  next_state.R = std::round(prev_state.R + m_gamma * prev_state.I);
  next_state.I = N() - prev_state.S - next_state.R;
  return next_state;
};

  Population approx(Population population_state) {
  long int tot = population_state.S + population_state.I + population_state.R;
  if (tot != N()) {
    long int diff = tot - N();
    if (diff > 0) {
      population_state.R -= diff;
    } else {
      population_state.I -= diff;
    }
  }
  return population_state;
};

 public:
  Epidemic(double beta, double gamma,
                   Population initial_population, long int T)
    : m_beta(beta), m_gamma(gamma), m_initial_population(initial_population), m_T(T) {
  assert(m_beta >= 0. && m_beta <= 1.);
  assert(m_gamma >= 0. && m_gamma <= 1.);
  assert(m_beta / m_gamma > 1);
  assert(m_initial_population.S > 0);
  assert(m_initial_population.I > 0);
  assert(m_initial_population.R > 0);
  assert(m_T > 0);
};

std::vector<Population> evolve() {
  std::vector<Population> population_state_;
  population_state_.push_back(m_initial_population);
  for (long int i = 0; i < T;) {
    if (population_state_[i].I < 0.6 * N()) {
      Population next_state = approx(solve(population_state_[i]));
      population_state_.push_back(next_state);
      ++i;
    } else {
      if (T - i < 14) {
        long int b = i;
        for (long int a = 0; a < T - i; ++a) {
          Population next_state = lockdown(population_state_[b]);
          population_state_.push_back(next_state);
          ++b;
        }
        i = b;
      } else {
        for (long int a = 0; a < 14; ++a) {
          Population next_state = lockdown(population_state_[i]);
          population_state_.push_back(next_state);
          ++i;
        }
      }
    }
  }
  return population_state_;
};

void graph() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Epidemic evolution"); 

    sf::RectangleShape xAxis(sf::Vector2f(window.getSize().x, 2.f));
    xAxis.setPosition(0.f, window.getSize().y / 2); 
    xAxis.setFillColor(sf::Color::Black); 

    sf::RectangleShape yAxis(sf::Vector2f(2.f, window.getSize().y));
    yAxis.setPosition(window.getSize().x / 2, 0.f);
    yAxis.setFillColor(sf::Color::Black);

    sf::Font font;
    sf::Text xAxisName("Days", font, 20);
    sf::Text yAxisName("Number of people", font, 20);
    xAxisName.setPosition(window.getSize().x - 100, window.getSize().y - 40);
    yAxisName.setPosition(20, 20);

        sf::Vector2f graphSize(600.f, 400.f);
    sf::Vector2f graphPosition(100.f, 100.f); 
    long int maxXValue = m_T + 1;
    long int maxYValue = N();
    for (float y = 0; y <= maxYValue; y += 20.f) {
      sf::Vertex line[] = {
      sf::Vertex(sf::Vector2f(graphPosition.x, graphPosition.y + graphSize.y - (y / maxYValue) * graphSize.y), sf::Color::Black),
          sf::Vertex(sf::Vector2f(graphPosition.x + graphSize.x,
                                  graphPosition.y + graphSize.y -
                                      (y / maxYValue) * graphSize.y),
                     sf::Color::Black)};
      window.draw(line, 2, sf::Lines);
    }
    for (float x = 0; x <= maxXValue; x += 2.f) {
      sf::Vertex line[] = {
          sf::Vertex(
              sf::Vector2f(graphPosition.x + (x / maxXValue) * graphSize.x,
                           graphPosition.y + graphSize.y),
              sf::Color::Black),
          sf::Vertex(
              sf::Vector2f(graphPosition.x + (x / maxXValue) * graphSize.x,
                           graphPosition.y),
              sf::Color::Black)};
      window.draw(line, 2, sf::Lines);
    }

    std::vector<sf::Text> xTicks;
    std::vector<sf::Text> yTicks;
    for (float x = 0.f; x <= maxXValue; x += 2.f) {
      sf::Text tickName;
      tickName.setFont(font);
      tickName.setCharacterSize(14);
      tickName.setFillColor(sf::Color::Black);
      tickName.setString(std::to_string(static_cast<int>(x)));
      tickName.setPosition(
          graphPosition.x + (x / maxXValue) * graphSize.x - 10,
          graphPosition.y + graphSize.y + 10);
      xTicks.push_back(tickName); 
    }
    for (float y = 0.f; y <= maxXValue; y += 20.f) {
      sf::Text tickName;
      tickName.setFont(font);
      tickName.setCharacterSize(14);
      tickName.setFillColor(sf::Color::Black);
      tickName.setString(std::to_string(static_cast<int>(
          y)));
      tickName.setPosition(graphPosition.x - 30,
                           graphPosition.y + graphSize.y -
                               (y / maxYValue) * graphSize.y -
                               7); 
      yTicks.push_back(
          tickName);
    }

    sf::VertexArray SusceptibleCurve(sf::LineStrip);  // creo le curve
    sf::VertexArray InfectedCurve(sf::LineStrip);
    sf::VertexArray RecoveryCurve(sf::LineStrip);
    std::vector<Population> population_state_ = evolve();
    for (int i = 0; i <= m_T; ++i) {
      int x = i;
      int yS = population_state_[i].S;
      int yI = population_state_[i].I;
      int yR = population_state_[i].R;
      SusceptibleCurve.append(sf::Vertex(
          sf::Vector2f(
              graphPosition.x + (x / maxXValue) * graphSize.x,
              graphPosition.y + graphSize.y - (yS / maxYValue) * graphSize.y),
          sf::Color::Blue));
      InfectedCurve.append(sf::Vertex(
          sf::Vector2f(
              graphPosition.x + (x / maxXValue) * graphSize.x,
              graphPosition.y + graphSize.y - (yI / maxYValue) * graphSize.y),
          sf::Color::Red));
      RecoveryCurve.append(sf::Vertex(
          sf::Vector2f(
              graphPosition.x + (x / maxXValue) * graphSize.x,
              graphPosition.y + graphSize.y - (yR / maxYValue) * graphSize.y),
          sf::Color::Green));
    }

    while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();
      }
      window.clear(sf::Color::White);

      window.draw(xAxis);
      window.draw(yAxis);
      window.draw(xAxisName);
      window.draw(yAxisName);
      for (auto tick = xTicks.begin(), last = xTick.end(); tick != last; ++tick) {
        window.draw(*tick);
      }
      for (auto tick = yTicks.begin(), last = yTick.end(); tick != last; ++tick) {
        window.draw(*tick);
      }
      window.draw(SusceptibleCurve);
      window.draw(InfectedCurve);
      window.draw(RecoveryCurve);
      window.display();
    }
  };
};

int main() {
  std::cout << "Please write initial population's groups S, I, R:\n";
  pf::Population initial_population;
  std::cin >> initial_population.S >> initial_population.I >>
      initial_population.R;

  std::cout << "Please write epidemic's parameter beta and gamma:\n";
  double beta;
  double gamma;
  std::cin >> beta >> gamma;
  std::cout << "Please write the duration of the epidemic T:\n";
  int T;
  std::cin >> T;
  
  pf::Epidemic epidemic(beta, gamma, initial_population, T);

  std::vector<pf::Population> population_state_ = epidemic.evolve();
  std::cout << "Report for each of the stored states of population:\n";
  std::cout << "Day  S    I   R \n";
  for (int i = 0; i <= T; ++i) {
    std::cout << i << "  " << population_state_[i].S << "  "
              << population_state_[i].I << "  " << population_state_[i].R
              << '\n';
  }

  graphic();
}

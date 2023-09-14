
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>

// puoi cambiare i colori mettendo #define RED sf::Color::Red

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

  long int N() const {
    return m_initial_population.S + m_initial_population.I +
           m_initial_population.R;
  };

  Population solve(Population prev_state) {
    Population next_state;
    next_state.S =
        std::round(prev_state.S - m_beta * prev_state.S * prev_state.I / N());
    next_state.I =
        std::round(prev_state.I + m_beta * prev_state.S * prev_state.I / N() -
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
  Epidemic(double beta, double gamma, Population initial_population, long int T)
      : m_beta(beta),
        m_gamma(gamma),
        m_initial_population(initial_population),
        m_T(T) {
    assert(m_beta >= 0. && m_beta <= 1.);
    assert(m_gamma >= 0. && m_gamma <= 1.);
    assert(m_beta / m_gamma > 1);
    assert(m_initial_population.S >= 0);
    assert(m_initial_population.I > 0);
    assert(m_initial_population.R >= 0);
    assert(m_T > 0);
  };

  std::vector<Population> evolve() {
    std::vector<Population> population_state_;
    population_state_.push_back(m_initial_population);
    for (long int i = 0; i < m_T;) {
      if (population_state_[i].I < 0.6 * N()) {
        Population next_state = approx(solve(population_state_[i]));
        population_state_.push_back(next_state);
        ++i;
      } else {
        if (m_T - i < 14) {
          long int b = i;
          for (long int a = 0; a < m_T - i; ++a) {
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
    // finestra su cui si stampa tutto
    sf::RenderWindow window(sf::VideoMode(1000, 640), "");

    // titolo al centro
    sf::Font font;
    font.loadFromFile("graph/text/Arialn.ttf");
    sf::Text Title("Epidemic Evolution", font, 25);
    Title.setFillColor(sf::Color::Black);
    Title.setPosition((window.getSize().x - 160.f) / 2, 30.f);

    // lato in basso esterno della griglia
    sf::VertexArray xAxis(sf::Lines, 2);
    xAxis[0].position = sf::Vector2f(80.f, window.getSize().y - 120.f);
    xAxis[1].position =
        sf::Vector2f(window.getSize().x - 80.f, window.getSize().y - 120.f);
    xAxis[0].color = sf::Color::Black;
    xAxis[1].color = sf::Color::Black;

    // lato a sinistra esterno della griglia
    sf::VertexArray yAxis(sf::Lines, 2);
    yAxis[0].position = sf::Vector2f(80.f, window.getSize().y - 120.f);
    yAxis[1].position = sf::Vector2f(80.f, 80.f);
    yAxis[0].color = sf::Color::Black;
    yAxis[1].color = sf::Color::Black;

    // lato in alto esterno della griglia
    sf::VertexArray Top(sf::Lines, 2);
    Top[0].position = sf::Vector2f(80.f, 80.f);
    Top[1].position = sf::Vector2f(window.getSize().x - 80.f, 80.f);
    Top[0].color = sf::Color::Black;
    Top[1].color = sf::Color::Black;

    // lato esterno a destra della griglia
    sf::VertexArray Right(sf::Lines, 2);
    Right[0].position =
        sf::Vector2f(window.getSize().x - 80.f, window.getSize().y - 120.f);
    Right[1].position = sf::Vector2f(window.getSize().x - 80.f, 80.f);
    Right[0].color = sf::Color::Black;
    Right[1].color = sf::Color::Black;

    // titolo asse x
    sf::Text xAxisName("Days", font, 20);
    xAxisName.setFillColor(sf::Color::Black);
    xAxisName.setPosition(window.getSize().x - 150, window.getSize().y - 80.f);

    // titolo asse y
    sf::Text yAxisName("Number of people", font, 20);
    yAxisName.setFillColor(sf::Color::Black);
    yAxisName.setPosition(15, 230);
    yAxisName.setRotation(-90.f);

    // righe verticali della griglia
    // devi cambiare vertical con orizontal!!
    std::vector<sf::VertexArray> OrizontalLines;
    for (int i = 1; i < 8; ++i) {
      sf::VertexArray OrizontalLine(sf::Lines, 2);
      OrizontalLine[0].position =
          sf::Vector2f(80.f + (window.getSize().x - 160.f) * i / 8,
                       window.getSize().y - 120.f);
      OrizontalLine[1].position =
          sf::Vector2f(80.f + (window.getSize().x - 160.f) * i / 8, 80.f);
      OrizontalLine[0].color = sf::Color(175, 238, 238);
      OrizontalLine[1].color = sf::Color(175, 238, 238);
      OrizontalLines.push_back(OrizontalLine);
    }

    // righe orizzontali della griglia
    std::vector<sf::VertexArray> VerticalLines;
    for (int i = 1; i < 6; ++i) {
      sf::VertexArray VerticalLine(sf::Lines, 2);
      VerticalLine[0].position =
          sf::Vector2f(80.f, 80.f + (window.getSize().y - 200.f) * i / 6);
      VerticalLine[1].position =
          sf::Vector2f(window.getSize().x - 80.f,
                       80.f + (window.getSize().y - 200.f) * i / 6);
      VerticalLine[0].color = sf::Color(175, 238, 238);
      VerticalLine[1].color = sf::Color(175, 238, 238);
      VerticalLines.push_back(VerticalLine);
    }

    // Curve --> sono giuste: non toccarle!
    long int maxXValue = m_T + 1;
    long int maxYValue = N();
    std::vector<Population> population_state_ = evolve();
    std::vector<sf::VertexArray> Susceptible;
    std::vector<sf::VertexArray> Infected;
    std::vector<sf::VertexArray> Removed;
    std::vector<sf::CircleShape> S_dots;
    std::vector<sf::CircleShape> I_dots;
    std::vector<sf::CircleShape> R_dots;
    float xscale = (window.getSize().x - 160.f) / maxXValue;
    float yscale = (window.getSize().y - 200.f) / maxYValue;
    for (int i = 0; i <= m_T; ++i) {
      sf::VertexArray SusceptibleCurve(sf::Lines, 2);
      sf::VertexArray InfectedCurve(sf::Lines, 2);
      sf::VertexArray RemovedCurve(sf::Lines, 2);
      SusceptibleCurve[0].position =
          sf::Vector2f(80.f + xscale * i, -120.f + window.getSize().y -
                                              yscale * population_state_[i].S);
      SusceptibleCurve[1].position = sf::Vector2f(
          80.f + xscale * (i + 1),
          -120.f + window.getSize().y - yscale * population_state_[i + 1].S);
      InfectedCurve[0].position =
          sf::Vector2f(80.f + xscale * i, -120.f + window.getSize().y -
                                              yscale * population_state_[i].I);
      InfectedCurve[1].position = sf::Vector2f(
          80.f + xscale * (i + 1),
          -120.f + window.getSize().y - yscale * population_state_[i + 1].I);
      RemovedCurve[0].position =
          sf::Vector2f(80.f + xscale * i, -120.f + window.getSize().y -
                                              yscale * population_state_[i].R);
      RemovedCurve[1].position = sf::Vector2f(
          80.f + xscale * (i + 1),
          -120.f + window.getSize().y - yscale * population_state_[i + 1].R);
      SusceptibleCurve[0].color = sf::Color(0, 0, 255);
      SusceptibleCurve[1].color = sf::Color(0, 0, 255);
      InfectedCurve[0].color = sf::Color(230, 0, 0);
      InfectedCurve[1].color = sf::Color(230, 0, 0);
      RemovedCurve[0].color = sf::Color(20, 230, 0);
      RemovedCurve[1].color = sf::Color(20, 230, 0);
      Susceptible.push_back(SusceptibleCurve);
      Infected.push_back(InfectedCurve);
      Removed.push_back(RemovedCurve);
      sf::CircleShape Sdots(4.f);
      sf::CircleShape Idots(4.f);
      sf::CircleShape Rdots(4.f);
      Sdots.setFillColor(sf::Color(0, 0, 255));
      Idots.setFillColor(sf::Color(230, 0, 0));
      Rdots.setFillColor(sf::Color(20, 230, 0));
      Sdots.setPosition(sf::Vector2f(80.f + xscale * (i + 1),
                                     -120.f + window.getSize().y -
                                         yscale * population_state_[i + 1].S -
                                         2));
      Idots.setPosition(sf::Vector2f(80.f + xscale * (i + 1),
                                     -120.f + window.getSize().y -
                                         yscale * population_state_[i + 1].I -
                                         2));
      Rdots.setPosition(sf::Vector2f(80.f + xscale * (i + 1),
                                     -120.f + window.getSize().y -
                                         yscale * population_state_[i + 1].R -
                                         2));
      S_dots.push_back(Sdots);
      I_dots.push_back(Idots);
      R_dots.push_back(Rdots);
    }

    // numeri sull'asse x
    std::vector<sf::Text> days;
    for (int i = 0; i <= 8; ++i) {
      std::string xstring = std::to_string(m_T * i / 8);
      sf::Text xnumber(xstring, font, 20);
      xnumber.setFillColor(sf::Color::Black);
      xnumber.setPosition(80.f + (window.getSize().x - 160.f) * i / 8,
                          window.getSize().y - 110);
      days.push_back(xnumber);
    }

    // numeri sull'asse y
    std::vector<sf::Text> people;
    for (int i = 0; i <= 6; ++i) {
      std::string ystring = std::to_string(N() * i / 6);
      sf::Text ynumber(ystring, font, 20);
      ynumber.setFillColor(sf::Color::Black);
      ynumber.setPosition(50.f, (window.getSize().y - 130.f) -
                                    (window.getSize().y - 200.f) * i / 6);
      people.push_back(ynumber);
    }

    // legenda

    // righe colorate
    std::vector<sf::VertexArray> lines;
    sf::VertexArray SusceptibleLine(sf::Lines, 2);
    SusceptibleLine[0].position =
        sf::Vector2f(200.f, window.getSize().y - 40.f);
    SusceptibleLine[1].position =
        sf::Vector2f(250.f, window.getSize().y - 40.f);
    SusceptibleLine[0].color = sf::Color(0, 0, 255);
    SusceptibleLine[1].color = sf::Color(0, 0, 255);
    lines.push_back(SusceptibleLine);

    sf::VertexArray InfectedLine(sf::Lines, 2);
    InfectedLine[0].position = sf::Vector2f(400.f, window.getSize().y - 40.f);
    InfectedLine[1].position = sf::Vector2f(450.f, window.getSize().y - 40.f);
    InfectedLine[0].color = sf::Color(230, 0, 0);
    InfectedLine[1].color = sf::Color(230, 0, 0);
    lines.push_back(InfectedLine);

    sf::VertexArray RemovedLine(sf::Lines, 2);
    RemovedLine[0].position = sf::Vector2f(600.f, window.getSize().y - 40.f);
    RemovedLine[1].position = sf::Vector2f(650.f, window.getSize().y - 40.f);
    RemovedLine[0].color = sf::Color(20, 230, 0);
    RemovedLine[1].color = sf::Color(20, 230, 0);
    lines.push_back(RemovedLine);

    // puntini
    std::vector<sf::CircleShape> Dots;
    sf::CircleShape Sdot(4.f);
    sf::CircleShape Idot(4.f);
    sf::CircleShape Rdot(4.f);
    Sdot.setFillColor(sf::Color(0, 0, 255));
    Idot.setFillColor(sf::Color(230, 0, 0));
    Rdot.setFillColor(sf::Color(20, 230, 0));
    Sdot.setPosition(223.f, window.getSize().y - 43.f);
    Idot.setPosition(423.f, window.getSize().y - 43.f);
    Rdot.setPosition(623.f, window.getSize().y - 43.f);
    Dots.push_back(Sdot);
    Dots.push_back(Idot);
    Dots.push_back(Rdot);

    // scritte
    std::vector<sf::Text> description;
    sf::Text desc1("Susceptible", font, 20);
    desc1.setFillColor(sf::Color::Black);
    desc1.setPosition(270.f, window.getSize().y - 50.f);
    description.push_back(desc1);
    sf::Text desc2("Infected", font, 20);
    desc2.setFillColor(sf::Color::Black);
    desc2.setPosition(470.f, window.getSize().y - 50.f);
    description.push_back(desc2);
    sf::Text desc3("Removed", font, 20);
    desc3.setFillColor(sf::Color::Black);
    desc3.setPosition(670.f, window.getSize().y - 50.f);
    description.push_back(desc3);

    while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();
      }
      window.clear(sf::Color::White);

      window.draw(Title);
      window.draw(xAxis);
      window.draw(yAxis);
      window.draw(Top);
      window.draw(Right);
      window.draw(xAxisName);
      window.draw(yAxisName);
      for (const sf::VertexArray& OrizontalLine : OrizontalLines) {
        window.draw(OrizontalLine);
      }
      for (const sf::VertexArray& VerticalLine : VerticalLines) {
        window.draw(VerticalLine);
      }
      for (const sf::VertexArray& SusceptibleCurve : Susceptible) {
        window.draw(SusceptibleCurve);
      }
      for (const sf::VertexArray& InfectedCurve : Infected) {
        window.draw(InfectedCurve);
      }
      for (const sf::VertexArray& RemovedCurve : Removed) {
        window.draw(RemovedCurve);
      }
      for (const sf::CircleShape& Sdots : S_dots) {
        window.draw(Sdots);
      }
      for (const sf::CircleShape& Idots : I_dots) {
        window.draw(Idots);
      }
      for (const sf::CircleShape& Rdots : R_dots) {
        window.draw(Rdots);
      }
      for (const sf::Text& xnumber : days) {
        window.draw(xnumber);
      }
      for (const sf::Text& ynumber : people) {
        window.draw(ynumber);
      }
      for (const sf::VertexArray& line : lines) {
        window.draw(line);
      }
      for (const sf::CircleShape& dots : Dots) {
        window.draw(dots);
      }
      for (const sf::Text& desc : description) {
        window.draw(desc);
      }
      window.display();
    }
  };

  int wrapValue(int v, int vMax) {
    if (v == -1) return vMax - 1;
    if (v == vMax) return 0;
    return v;
  };

  double probability(int n) {
    double result = 1 - std::pow((1 - m_beta), n);
    return result;
  };

  void automa_cellulare() {
    // creo finestra grafica
    sf::RenderWindow window(sf::VideoMode(1250, 1000), "Cellular Automaton");

    // creo griglia
    int Parts = 1;
    for (; Parts * Parts < N(); ++Parts) {
    }

    const int CellLenght =
        (window.getSize().x - 258) / Parts;  // i margini sono spessi 4
    const sf::Vector2f CELL_VECTOR(CellLenght, CellLenght);
    const int N_cells = Parts * Parts;
    int grid[N_cells] = {};
    int gridNext[N_cells];

    std::vector<int> CellNumber;  // III
    for (int i = 0; i < N_cells; ++i) {
      CellNumber.push_back(i);
    }

    auto it = CellNumber.begin();
    auto last = CellNumber.end();

    random_shuffle(
        it,
        last);  // così facendo le celle non cambiano posizione da un avvio
                // all'altro, dati i parametri iniziali l'inizio sarà lo stesso

    for (int i = 0; i < N_cells; i++) {
      if (i < m_initial_population.S) {
        grid[CellNumber[i]] = 0;
      } else if (i < m_initial_population.S + m_initial_population.I) {
        grid[CellNumber[i]] = 1;
      } else if (i < m_initial_population.S + m_initial_population.I +
                         m_initial_population.R) {
        grid[CellNumber[i]] = 2;
      } else {
        grid[CellNumber[i]] = 3;
      }
    }

    sf::Font font;
    font.loadFromFile("graph/text/Arialn.ttf");
    sf::Text text;
    text.setFillColor(sf::Color::Black);
    text.setPosition(window.getSize().x - 230, 40);
    text.setFont(font);
    text.setCharacterSize(30);
    int dayCounter = 0;
    int SCounter = m_initial_population.S;
    int ICounter = m_initial_population.I;
    int RCounter = m_initial_population.R;
    text.setString("Day = " + std::to_string(dayCounter) +
                       "\n\nSusceptible = " + std::to_string(SCounter) +
                       "\n\nInfected = " + std::to_string(ICounter) +
                       "\n\nRemoved = " + std::to_string(RCounter));

    sf::Clock clock;

    // disegna
    while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();
      }

      if (clock.getElapsedTime().asSeconds() > 1.0f) {
        clock.restart();
        dayCounter++;
        SCounter = 0;
        for (int x = 0; x < Parts; x++) {
          for (int y = 0; y < Parts; y++) {
            if (grid[x + y * Parts] == 0) {
              SCounter++;
            }
          }
        }
        ICounter = 0;
        for (int x = 0; x < Parts; x++) {
          for (int y = 0; y < Parts; y++) {
            if (grid[x + y * Parts] == 1) {
              ICounter++;
            }
          }
        }
        RCounter = 0;
        for (int x = 0; x < Parts; x++) {
          for (int y = 0; y < Parts; y++) {
            if (grid[x + y * Parts] == 2) {
              RCounter++;
            }
          }
        }

        text.setString("Day = " + std::to_string(dayCounter) +
                       "\n\nSusceptible = " + std::to_string(SCounter) +
                       "\n\nInfected = " + std::to_string(ICounter) +
                       "\n\nRemoved = " + std::to_string(RCounter));
      }

      window.clear(sf::Color::White);
      for (int x = 0; x < Parts; x++) {
        for (int y = 0; y < Parts; y++) {
          // draw cell
          sf::RectangleShape cell;
          cell.setPosition(x * CellLenght + 5, y * CellLenght + 5);
          cell.setSize(CELL_VECTOR);
          cell.setOutlineThickness(2);
          cell.setOutlineColor(sf::Color::Black);
          if (grid[x + y * Parts] == 0) {
            cell.setFillColor(sf::Color::Blue);
          } else if (grid[x + y * Parts] == 1) {
            cell.setFillColor(sf::Color::Red);
          } else if (grid[x + y * Parts] == 2) {
            cell.setFillColor(sf::Color::Green);
          } else if (grid[x + y * Parts] == 3) {
            cell.setFillColor(sf::Color::White);
          }

          window.draw(cell);

          // prepare gridNext
          int neighborSum = 0;

          int current = x + y * Parts;
          gridNext[current] = grid[current];
          srand(time(NULL));
          if (grid[current] == 0) {
            for (int i = -1; i < 2; i++) {
              for (int j = -1; j < 2; j++) {
                int xi = wrapValue(x + i, Parts);
                int yj = wrapValue(y + j, Parts);
                if (grid[xi + yj * Parts] == 2) {
                  grid[xi + yj * Parts] = 0;
                  neighborSum += grid[xi + yj * Parts];
                  grid[xi + yj * Parts] = 2;
                } else if (grid[xi + yj * Parts] == 3) {
                  grid[xi + yj * Parts] = 0;
                  neighborSum += grid[xi + yj * Parts];
                  grid[xi + yj * Parts] = 3;
                } else {
                  neighborSum += grid[xi + yj * Parts];
                }
              }
            }
            int probNumber = rand() % 100;  // perchè non rialcola una nuova
                                            // probabilità ad ogni passaggio?
            double prob = probability(neighborSum);
            if (probNumber < prob * 100) {
              gridNext[current] = 1;
            }
          } else if (grid[current] == 1) {
            int probNumber = rand() % 100;
            if (probNumber < m_gamma * 100) {
              gridNext[current] = 2;
            }
          }
        }
      }

      // move gridNext to grid
      for (int i = 0; i < N_cells; i++) {
        grid[i] = gridNext[i];
      }

      window.draw(text);

      window.display();
    }
  };
};

int main() {
  Population initial_population{58, 17, 8};
  Epidemic epidemic{0.25, 0.15, initial_population, 25};
  /*
        std::cout << "Please write initial population's groups S, I, R:\n";
        Population initial_population;
        std::cin >> initial_population.S >> initial_population.I >>
            initial_population.R;

        std::cout << "Please write epidemic's parameter beta and gamma:\n";
        double beta;
        double gamma;
        std::cin >> beta >> gamma;

        std::cout << "Please write the duration of the epidemic T:\n";
        int T;
        std::cin >> T;

        Epidemic epidemic{beta, gamma, initial_population, T};


    std::vector<Population> population_state_ = epidemic.evolve();
    std::cout << "Report for each of the stored states of population:\n";
    std::cout << "Day  S    I   R \n";
    for (int i = 0; i <= T; ++i) {
      std::cout << i << "  " << population_state_[i].S << "  "
                << population_state_[i].I << "  " << population_state_[i].R
                << '\n';}*/

  // epidemic.graph();
  /*
    std::vector<int> vec = epidemic.vector();
    std::cout << vec.size() << "\n\n";
    for (int i = 0; i < vec.size(); ++i) {
      std::cout << vec[i] << '\n';
    }*/

  epidemic.automa_cellulare();
}

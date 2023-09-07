#include <iostream>

#include "functions/functions.hpp"

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

  pf::Epidemic epidemic{beta, gamma, initial_population, T};

  std::vector<pf::Population> population_state_ = epidemic.evolve();
  std::cout << "Report for each of the stored states of population:\n";
  std::cout << "Day  S    I   R \n";
  for (int i = 0; i <= T; ++i) {
    std::cout << i << "  " << population_state_[i].S << "  "
              << population_state_[i].I << "  " << population_state_[i].R
              << '\n';
  }
}

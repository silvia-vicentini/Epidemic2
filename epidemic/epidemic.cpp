#include "epidemic.hpp"

#include <cassert>
#include <cmath>
#include <vector>

namespace pf {

// constructor
Epidemic::Epidemic(double beta, double gamma,
                   Population<double> initial_population, int T)
    : beta_(beta),
      gamma_(gamma),
      initial_population_(initial_population),
      T_(T) {
  assert(beta_ >= 0. && beta_ <= 1.);
  assert(gamma_ >= 0. && gamma_ <= 1.);
  assert(beta_ / gamma_ > 1);
  assert(initial_population_.S >= 0);
  assert(initial_population_.I >= 0);
  assert(initial_population_.R >= 0);
  assert(T_ > 0);

  std::vector<Population<double>> simulation_double_ = Epidemic::store();
}

// private methods
Population<double> Epidemic::calculate(Population<double> prev_state) {
  Population<double> next_state;
  next_state.S = prev_state.S - beta_ * prev_state.S * prev_state.I / N();
  next_state.I = prev_state.I + beta_ * prev_state.S * prev_state.I / N() -
                 gamma_ * prev_state.I;
  next_state.R = prev_state.R + gamma_ * prev_state.I;
  return next_state;
}

Population<double> Epidemic::lockdown(Population<double> prev_state) {
  Population<double> next_state;
  next_state.S = prev_state.S;
  next_state.R = prev_state.R + gamma_ * prev_state.I;
  next_state.I = N() - prev_state.S - next_state.R;
  return next_state;
}

std::vector<Population<double>> Epidemic::store() {
  std::vector<Population<double>> simulation_double{initial_population_};
  for (int i = 0; i < T_;) {
    if (simulation_double[i].I < 0.6 * N()) {
      Population<double> next_state = calculate(simulation_double[i]);
      simulation_double.push_back(next_state);
      ++i;
    } else {
      if (T_ - i < 14) {
        int b = i;
        for (int a = 0; a < T_ - i; ++a) {
          Population<double> next_state = lockdown(simulation_double[b]);
          simulation_double.push_back(next_state);
          ++b;
        }
        i = b;
      } else {
        for (int a = 0; a < 14; ++a) {
          Population<double> next_state = lockdown(simulation_double[i]);
          simulation_double.push_back(next_state);
          ++i;
        }
      }
    }
  }
  return simulation_double;
}

Population<int> Epidemic::round(Population<double> double_state) {
  Population<int> int_state;
  int_state.S = std::round(double_state.S);
  int_state.I = std::round(double_state.I);
  int_state.R = std::round(double_state.R);
  return int_state;
}

Population<int> Epidemic::approx(Population<int> population_state) {
  int tot = population_state.S + population_state.I + population_state.R;
  if (tot != N()) {
    int diff = tot - N();
    if (diff > 0) {
      population_state.R -= diff;
    } else {
      population_state.I -= diff;
    }
  }
  return population_state;
}

// public methods
std::vector<Population<int>> Epidemic::evolution() {
  std::vector<Population<int>> simulation_int_;
  for (auto population_state_double : simulation_double_) {
    Population<int> population_state_int =
        approx(round(population_state_double));
    simulation_int_.push_back(population_state_int);
  }
  return simulation_int_;
}

std::vector<Population<double>> Epidemic::get_simulation_double() const {
  return simulation_double_;
}
int Epidemic::N() const {
  return initial_population_.S + initial_population_.I + initial_population_.R;
}
int Epidemic::getT() const { return T_; }

}  // namespace pf

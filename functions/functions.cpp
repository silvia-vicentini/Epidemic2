#include "functions.hpp"

#include <cassert>
#include <cmath>
#include <vector>

namespace pf {

Epidemic::Epidemic(double const beta, double const gamma,
                   Population initial_population)
    : m_beta(beta), m_gamma(gamma), m_initial_population(initial_population) {
  assert(m_beta >= 0. && m_beta <= 1.);
  assert(m_gamma >= 0. && m_gamma <= 1.);
  assert(m_beta / m_gamma > 1);
  assert(m_initial_population.S > 0);
  assert(m_initial_population.I > 0);
  assert(m_initial_population.R > 0);
}

long int Epidemic::N() const{
  return m_initial_population.S + m_initial_population.I +
         m_initial_population.R;
}

// definition of solve function
Population Epidemic::solve(Population prev_state) {
  Population next_state;
  next_state.S = std::round(prev_state.S - m_beta * prev_state.S *
                                               prev_state.I / Epidemic::N());
  next_state.I = std::round(
      prev_state.I + m_beta * prev_state.S * prev_state.I / Epidemic::N() -
      m_gamma * prev_state.I);
  next_state.R = std::round(prev_state.R + m_gamma * prev_state.I);
  return next_state;
}

// definition of lockdown function
Population Epidemic::lockdown(Population prev_state) {
  Population next_state;
  next_state.S = prev_state.S;
  next_state.R = std::round(prev_state.R + m_gamma * prev_state.I);
  next_state.I = Epidemic::N() - prev_state.S - next_state.R;
  return next_state;
}
/*
// se si fanno dei vaccini parte dei sani entra nei R perché diventano immuni
Population vaccine(Population prev_state,  N, double beta,
                   double gamma) {
  Population next_state;
  next_state.S = std::round(prev_state.S -
                            beta * prev_state.S * prev_state.I / N - 0.05 *
N); next_state.R = std::round(prev_state.R + gamma * prev_state.I + 0.05 * N);
  next_state.I =
      std::round(prev_state.I + beta * prev_state.S * prev_state.I / N -
                 gamma * prev_state.I);
  return next_state;
}*/

Population Epidemic::approx(Population population_state) {
  long int tot = population_state.S + population_state.I + population_state.R;
  if (tot != Epidemic::N()) {
    long int diff = tot - Epidemic::N();
    if (diff > 0) {
      population_state.R -= diff;
    } else {
      population_state.I -= diff;
    }
  }
  return population_state;
}

std::vector<Population> Epidemic::evolve(long int T) {
  std::vector<Population> population_state_;
  population_state_.push_back(m_initial_population);
  for (long int i = 0; i < T;) {
    if (population_state_[i].I < 0.6 * Epidemic::N()) {
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
  /*else {
    auto next_state =
        approx(vaccine(population_state_[i], N, beta, gamma), N);
    population_state_.push_back(next_state);
  }
  }*/
  return population_state_;
}
}  // namespace pf

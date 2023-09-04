#ifndef EPIDEMIC_HPP
#define EPIDEMIC_HPP
#include <vector>

namespace pf {

struct Population {
  long int S;
  long int I;
  long int R;
};

class Epidemic {
 private:
  double const m_beta;
  double const m_gamma;
  Population m_initial_population;

 public:
  Epidemic(double const, double const, Population);

  long int N();

  Population solve(Population);

  Population lockdown(Population);

  Population approx(Population);

  std::vector<Population> evolve(long int);
};
}  // namespace pf

#endif

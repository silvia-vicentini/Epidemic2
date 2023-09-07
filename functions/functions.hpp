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
  double m_beta;
  double m_gamma;
  Population m_initial_population;
  long int m_T;

  long int N() const;

  Population solve(Population);

  Population lockdown(Population);

  Population approx(Population);

 public:
  Epidemic(double, double, Population, long int);

  std::vector<Population> evolve();
};
}  // namespace pf

#endif

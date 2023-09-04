#ifndef EPIDEMIC_HPP
#define EPIDEMIC_HPP

namespace pf {

struct Population {
  long int S;
  long int I;
  long int R;
};

class Epidemic {
 private:
  double const beta_;
  double const gamma_;
  Population initial_population_;

 public:
  Epidemic(double const, double const, Population);

  long int N();

  Population solve(Population);

  Population lockdown(Population);

  Population approx(Population);
  
  std::vector<Population> evolve(Population, long int);
};
}  // namespace pf

#endif

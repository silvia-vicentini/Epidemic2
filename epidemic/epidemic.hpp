#ifndef EPIDEMIC_HPP
#define EPIDEMIC_HPP
#include <vector>

namespace pf {

template <typename type>
struct Population {
  type S;
  type I;
  type R;
};

class Epidemic {
 private:
  double beta_;
  double gamma_;
  Population<double> initial_population_;
  int T_;
  std::vector<Population<double>> simulation_double_;

  // calcola S I e R il giorno successivi registrando i valori come double
  Population<double> calculate(Population<double>);

  // calcola S I e R il giorno successivi registrando i valori come double nel
  // caso in cui si avvii un lockdown
  Population<double> lockdown(Population<double>);

  // inserisce ogni stato della popolazione giornaliero all'interno del vettore
  std::vector<Population<double>> store();

  // arritonda i valori di S I e R di ciascun giorno in interi
  Population<int> round(Population<double>);

  // arrotonda S I e R in modo tale che la loro somma sia sempre pari a N
  Population<int> approx(Population<int>);

 public:
  // costruttore
  Epidemic(double, double, Population<double>, int);

  std::vector<Population<int>> evolution();

  std::vector<Population<double>> get_simulation_double() const;

  // std::vector<Population<double>> get_simulation_double();
  int N() const;
  int getT() const;
};
}  // namespace pf

#endif

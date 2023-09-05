#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "functions.hpp"

#include "doctest.h"

// mancano i test su vaccine!!

bool result(pf::Population p, pf::Population v) {
  return p.S == v.S && p.I == v.I && p.R == v.R;
}

TEST_CASE("Testing the approximations") {
  SUBCASE("For tot > N, case 1") {
    pf::Population initial_population{997, 3, 0};
    pf::Epidemic epidemic(0.8, 0.4, initial_population, 8);
    const auto state = epidemic.evolve();
    CHECK(result(state[4], {980, 13, 7}));
    CHECK(result(state[8], {912, 46, 42}));
  }

  SUBCASE("For tot > N, case 2") {
    pf::Population initial_population{997, 3, 0};
    pf::Epidemic epidemic(0.8, 0.27, initial_population, 25);
    const auto state = epidemic.evolve();
    CHECK(result(state[1], {995, 5, 0}));
    CHECK(result(state[16], {160, 290, 550}));
    CHECK(result(state[25], {49, 39, 912}));
  }

  SUBCASE("For tot < N, case 1") {
    pf::Population initial_population{997, 3, 0};
    pf::Epidemic epidemic(0.8, 0.4, initial_population, 20);
    const auto state = epidemic.evolve();
    CHECK(result(state[9], {878, 62, 60}));
    CHECK(result(state[17], {365, 163, 472}));
  }

  SUBCASE("For tot < N, case 2") {
    pf::Population initial_population{997, 3, 0};
    pf::Epidemic epidemic(0.8, 0.27, initial_population, 25);
    const auto state = epidemic.evolve();
    CHECK(result(state[6], {941, 41, 18}));
    CHECK(result(state[10], {722, 175, 103}));
    CHECK(result(state[18], {98, 207, 695}));
  }
}

TEST_CASE("Testing the evolve function") {
  SUBCASE("Normal use, beta -> gamma, N=300") {
    pf::Population initial_population{299, 1, 0};
    pf::Epidemic epidemic(0.99, 0.01, initial_population, 15);
    const auto state = epidemic.evolve();
    CHECK(result(state[0], {299, 1, 0}));
    CHECK(result(state[1], {298, 1, 1}));
    CHECK(result(state[2], {297, 1, 2}));
    CHECK(result(state[3], {296, 1, 3}));
    CHECK(result(state[4], {295, 1, 4}));
    CHECK(result(state[13], {286, 1, 13}));
    CHECK(result(state[14], {285, 1, 14}));
    CHECK(result(state[15], {284, 1, 15}));
  }

  SUBCASE("Normal use, beta >> gamma, N=300") {
    pf::Population initial_population{299, 1, 0};
    pf::Epidemic epidemic(0.99, 0.01, initial_population, 9);
    const auto state = epidemic.evolve();
    CHECK(result(state[0], {299, 1, 0}));
    CHECK(result(state[1], {298, 2, 0}));
    CHECK(result(state[2], {296, 4, 0}));
    CHECK(result(state[3], {292, 8, 0}));
    CHECK(result(state[4], {284, 16, 0}));
    CHECK(result(state[9], {55, 241, 4}));
  }

  SUBCASE("Beginning of Lockdown") {
    pf::Population initial_population{299, 1, 0};
    pf::Epidemic epidemic(0.99, 0.01, initial_population, 30);
    const auto state = epidemic.evolve();
    CHECK(result(state[9], {55, 241, 4}));
    CHECK(result(state[10], {55, 239, 6}));
    CHECK(result(state[11], {55, 237, 8}));
    CHECK(result(state[12], {55, 235, 10}));
    CHECK(result(state[23], {55, 213, 32}));
    CHECK(result(state[24], {55, 211, 34}));
    CHECK(result(state[25], {55, 209, 36}));
  }

  SUBCASE("Chages from normal use and lockdown") {
    pf::Population initial_population{299, 1, 0};
    pf::Epidemic epidemic(0.99, 0.01, initial_population, 60);
    const auto state = epidemic.evolve();
    CHECK(result(state[39], {55, 181, 64}));
    CHECK(result(state[50], {55, 159, 86}));
    CHECK(result(state[51], {55, 157, 88}));
    CHECK(result(state[52], {27, 184, 89}));
    CHECK(result(state[53], {27, 182, 91}));
  }

  SUBCASE("End of lockdown") {
    pf::Population initial_population{299, 1, 0};
    pf::Epidemic epidemic(0.99, 0.01, initial_population, 80);
    const auto state = epidemic.evolve();
    CHECK(result(state[66], {27, 156, 117}));
    CHECK(result(state[67], {13, 168, 119}));
    CHECK(result(state[80], {0, 157, 143}));
  }
}

TEST_CASE("Testing the evolve function for the italian population") {
  pf::Population initial_population{59109968, 32, 0};
  pf::Epidemic epidemic(0.186, 0.166, initial_population, 100);
  const auto state = epidemic.evolve();
  CHECK(result(state[0], {59109968, 32, 0}));
  CHECK(result(state[50], {59109420, 97, 483}));
  CHECK(result(state[100], {59107863, 269, 1868}));
  CHECK(result(state[150], {59103601, 731, 5668}));
  CHECK(result(state[200], {59092043, 1981, 15976}));
  CHECK(result(state[250], {59060942, 5314, 43744}));
  CHECK(result(state[300], {58977938, 14134, 117928}));
}

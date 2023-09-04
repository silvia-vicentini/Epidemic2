#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "functions.hpp"

#include "doctest.h"

// mancano i test su vaccine!!

bool result(pf::Population p, pf::Population v) {
  return p.S == v.S && p.I == v.I && p.R == v.R;
}

TEST_CASE("Testing the approximations") {
  SUBCASE("Testing the solve function: case 1") {
    pf::Population initial_population{187, 4, 0};
    pf::Epidemic epidemic(0.6, 0.2, initial_population);
    int N = 191;
    auto result = epidemic.solve(initial_population);
    CHECK(result.S == 186);
    CHECK(result.I == 5);
    CHECK(result.R == 1);
  }

  SUBCASE("Testing the solve function: case 2") {
    pf::Population initial_population{187, 4, 0};
    pf::Epidemic epidemic(0.65, 0.05, initial_population);
    int N = 191;
    auto result = epidemic.solve(initial_population);
    CHECK(result.S == 185);
    CHECK(result.I == 5);
    CHECK(result.R == 0);
  }

  SUBCASE("Testing the approx function for tot > N") {
    pf::Population initial_population{187, 4, 0};
    pf::Epidemic epidemic(0.6, 0.2, initial_population);
    int N = 191;
    auto result = epidemic.approx(initial_population);
    CHECK(result.S == 186);
    CHECK(result.I == 5);
    CHECK(result.R == 0);
  }

  SUBCASE("Testing the approx function for tot < N") {
    pf::Population initial_population{187, 4, 0};
    pf::Epidemic epidemic(0.65, 0.05, initial_population);
    int N = 191;
    auto result = epidemic.approx(initial_population);
    CHECK(result.S == 185);
    CHECK(result.I == 6);
    CHECK(result.R == 0);
  }
}

TEST_CASE("Testing the lockdown function") {
  pf::Population initial_population{45, 210, 45};
  pf::Epidemic epidemic(0.99, 0.01, initial_population);
  int N = 300;
  auto result = epidemic.lockdown(initial_population);
  CHECK(result.S == 45);
  CHECK(result.I == 189);
  CHECK(result.R == 66);
}
/*
TEST_CASE("Testing the vaccine function") {
  Epidemic epidemic{0.55, 0.4};
  Population initial_population{5076, 45, 4879};
  int N = 10000;
  auto result = epidemic.lockdown(initial_population, N);
  CHECK(result.S == 4810);
  CHECK(result.I == 40);
  CHECK(result.R == 5150);
}*/

TEST_CASE("Testing the evolve function") {
  SUBCASE("Normal use, beta -> gamma, N=300") {
    pf::Population initial_population{299, 1, 0};
    pf::Epidemic epidemic(0.99, 0.01, initial_population);
    int const T{15};
    const auto state = epidemic.evolve(T);
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
    pf::Epidemic epidemic(0.99, 0.01, initial_population);
    int const T{15};
    const auto state = epidemic.evolve(T);
    CHECK(result(state[0], {299, 1, 0}));
    CHECK(result(state[1], {298, 2, 0}));
    CHECK(result(state[2], {296, 4, 0}));
    CHECK(result(state[3], {292, 8, 0}));
    CHECK(result(state[4], {284, 16, 0}));
    CHECK(result(state[13], {0, 285, 15}));
    CHECK(result(state[14], {0, 282, 18}));
    CHECK(result(state[15], {0, 279, 21}));
  }
}

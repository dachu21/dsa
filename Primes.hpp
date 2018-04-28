#ifndef DIGITALSIGNATUREDES_FUNCTIONS_HPP
#define DIGITALSIGNATUREDES_FUNCTIONS_HPP

#include "BigInt.hpp"
#include "constants.hpp"

namespace Primes {

static bool trialDivision(const BigInt &n) {
  const int *p = &PRIMES[0];
  for (int i = 0; i < PRIMES_COUNT - 1; ++i) {
    if (n % *(p++) == 0) return true;
  }

  return false;
}

static bool millerRabin(const BigInt &n, int k) {
  BigInt d = n - 1;
  int r = 0;
  while (d % 2 == 0) {
    r++;
    d /= 2;
  }

  for (int i = 0; i < k; i++) {
    BigInt a = xrand(2, n - 1);
    BigInt x = modPow(a, d, n);

    if (x == 1 || x == n - 1) continue;

    int j;
    for (j = 0; j < r - 1; j++) {
      x = modPow(x, 2, n);
      if (x == 1) return false;
      if (x == n - 1) break;
    }

    if (j >= r - 1)
      return false;
  }

  return true;
}

static BigInt findRandomPrime(int confidence, const BigInt &min, const BigInt &max) {
  BigInt candidate;

  do candidate = xrand(min, max);
  while (trialDivision(candidate));

  while (!millerRabin(candidate, confidence)) {
    do candidate = xrand(min, max);
    while (trialDivision(candidate));
  }

  return candidate;
}

};

#endif //DIGITALSIGNATUREDES_FUNCTIONS_HPP
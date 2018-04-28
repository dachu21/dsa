#ifndef DIGITALSIGNATUREDES_ALGORITHMPARAMETERS_HPP
#define DIGITALSIGNATUREDES_ALGORITHMPARAMETERS_HPP

#include <fstream>

#include "BigInt.hpp"
#include "constants.hpp"
#include "Primes.hpp"

class AlgorithmParameters {

 private:
  void setQandP() {
    this->q = Q_GENERATED;
    this->p = P_GENERATED;
  }

  void generateQandP() {
    BigInt q, p;
    do {
      q = Primes::findRandomPrime(5, MIN_160_BIT, MAX_160_BIT);
      p = q * fastPow(2, L - N) + 1;
    } while (!Primes::millerRabin(p, 5));
    this->q = q;
    this->p = p;
  }

  void setOrGenerateQandP() {
    if (Q_GENERATED != 0 && P_GENERATED != 0) {
      setQandP();
    } else {
      generateQandP();
    }
  }

  void generateG() {
    BigInt h, g = BigInt("1");
    while (g == 1) {
      h = xrand(2, p - 2);
      g = modPow(h, (p - 1) / q, p);
    }
    this->g = g;
  }

 public:
  AlgorithmParameters() = default;

  BigInt p;
  BigInt q;
  BigInt g;

  void saveToFile(const std::string &filename) {
    std::fstream file;
    file.open(filename, std::ios::out | std::ios::trunc);
    if (file.good()) {
      file << this->p << std::endl << this->q << std::endl << this->g;
      file.close();
    }
  }

  static AlgorithmParameters loadFromFile(const std::string &filename) {
    AlgorithmParameters parameters;
    std::fstream file;
    file.open(filename, std::ios::in);
    if (file.good()) {
      file >> parameters.p >> parameters.q >> parameters.g;
      file.close();
    }
    return parameters;
  }

  static AlgorithmParameters generate() {
    AlgorithmParameters parameters;
    parameters.setOrGenerateQandP();
    parameters.generateG();
    return parameters;
  }
};

#endif //DIGITALSIGNATUREDES_ALGORITHMPARAMETERS_HPP
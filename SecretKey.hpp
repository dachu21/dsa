#ifndef DIGITALSIGNATUREDES_PRIVATEKEY_HPP
#define DIGITALSIGNATUREDES_PRIVATEKEY_HPP

#include <fstream>

#include "BigInt.hpp"
#include "constants.hpp"
#include "AlgorithmParameters.hpp"

class SecretKey {

 private:
  void generateX() {
    this->x = xrand(MIN_SECRET_KEY, q - 1);
  }

 public:
  SecretKey() = default;

  BigInt p;
  BigInt q;
  BigInt g;
  BigInt x;

  void saveToFile(const std::string &filename) {
    std::fstream file;
    file.open(filename, std::ios::out | std::ios::trunc);
    if (file.good()) {
      file << this->p << std::endl << this->q << std::endl << this->g << std::endl << this->x;
      file.close();
    }
  }

  static SecretKey loadFromFile(const std::string &filename) {
    SecretKey secretKey;
    std::fstream file;
    file.open(filename, std::ios::in);
    if (file.good()) {
      file >> secretKey.p >> secretKey.q >> secretKey.g >> secretKey.x;
      file.close();
    }
    return secretKey;
  }

  static SecretKey generate(const AlgorithmParameters &parameters) {
    SecretKey secretKey;
    secretKey.p = parameters.p;
    secretKey.q = parameters.q;
    secretKey.g = parameters.g;
    secretKey.generateX();
    return secretKey;
  }
};

#endif //DIGITALSIGNATUREDES_PRIVATEKEY_HPP
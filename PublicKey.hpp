#ifndef DIGITALSIGNATUREDES_PUBLICKEY_HPP
#define DIGITALSIGNATUREDES_PUBLICKEY_HPP

#include <fstream>

#include "BigInt.hpp"
#include "constants.hpp"
#include "SecretKey.hpp"

class PublicKey {

 private:
  void generateY(const BigInt &x) {
    this->y = modPow(g, x, p);
  }

 public:
  PublicKey() = default;

  BigInt p;
  BigInt q;
  BigInt g;
  BigInt y;

  void saveToFile(const std::string &filename) {
    std::fstream file;
    file.open(filename, std::ios::out | std::ios::trunc);
    if (file.good()) {
      file << this->p << std::endl << this->q << std::endl << this->g << std::endl << this->y;
      file.close();
    }
  }

  static PublicKey loadFromFile(const std::string &filename) {
    PublicKey publicKey;
    std::fstream file;
    file.open(filename, std::ios::in);
    if (file.good()) {
      file >> publicKey.p >> publicKey.q >> publicKey.g >> publicKey.y;
      file.close();
    }
    return publicKey;
  }

  static PublicKey generate(const SecretKey &secretKey) {
    PublicKey publicKey;
    publicKey.p = secretKey.p;
    publicKey.q = secretKey.q;
    publicKey.g = secretKey.g;
    publicKey.generateY(secretKey.x);
    return publicKey;
  }
};

#endif //DIGITALSIGNATUREDES_PUBLICKEY_HPP
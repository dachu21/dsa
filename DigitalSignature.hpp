#ifndef DIGITALSIGNATUREDES_DIGITALSIGNATURE_HPP
#define DIGITALSIGNATUREDES_DIGITALSIGNATURE_HPP

#include <fstream>

#include "BigInt.hpp"
#include "PublicKey.hpp"
#include "Message.hpp"

class DigitalSignature {

 private:
  DigitalSignature(const BigInt &r, const BigInt &s) : r(r), s(s) {}

 public:
  DigitalSignature() = default;

  BigInt r;
  BigInt s;

  void saveToFile(const std::string &filename) {
    std::fstream file;
    file.open(filename, std::ios::out | std::ios::trunc);
    if (file.good()) {
      file << this->r << std::endl << this->s;
      file.close();
    }
  }

  static DigitalSignature loadFromFile(const std::string &filename) {
    DigitalSignature signature;
    std::fstream file;
    file.open(filename, std::ios::in);
    if (file.good()) {
      file >> signature.r >> signature.s;
      file.close();
    }
    return signature;
  }

  static DigitalSignature sign(const SecretKey &secretKey, Message &message) {
    BigInt k, r, s;
    BigInt hash = message.hashWithSha512();
    do {
      k = xrand(BigInt("2"), secretKey.q - 1);
      r = modPow(secretKey.g, k, secretKey.p) % secretKey.q;
      s = (modPow(k, secretKey.q - 2, secretKey.q)
          * ((hash + secretKey.x * r) % secretKey.q))
          % secretKey.q;
    } while (r == 0 || s == 0);
    return DigitalSignature(r, s);
  }

  static bool verify(const PublicKey &publicKey,
                     Message &message,
                     const DigitalSignature &signature) {
    if (!(signature.r > 0 && signature.r < publicKey.q
        && signature.s > 0 && signature.s < publicKey.q)) {
      return false;
    }
    BigInt hash = message.hashWithSha512();
    BigInt w = modPow(signature.s, publicKey.q - 2, publicKey.q);
    BigInt u1 = ((hash % publicKey.q) * (w % publicKey.q)) % publicKey.q;
    BigInt u2 = ((signature.r % publicKey.q) * (w % publicKey.q)) % publicKey.q;
    BigInt v = ((modPow(publicKey.g, u1, publicKey.p)
        * modPow(publicKey.y, u2, publicKey.p))
        % publicKey.p)
        % publicKey.q;
    return v == signature.r;
  }
};

#endif //DIGITALSIGNATUREDES_DIGITALSIGNATURE_HPP
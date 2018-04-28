#ifndef DIGITALSIGNATUREDES_MESSAGE_HPP
#define DIGITALSIGNATUREDES_MESSAGE_HPP

#include <vector>
#include <string>
#include <fstream>

#include "BigInt.hpp"
#include "lib/sha-2/sha2.h"

class Message {

 private:
  explicit Message(std::vector<uint8_t> buffer) : buffer(std::move(buffer)) {}

 public:
  Message() = default;

  std::vector<uint8_t> buffer;

  BigInt hashWithSha512() {
    Sha512 hasher;
    uint8_t *input = &buffer[0];
    std::vector<uint8_t> output;
    hasher.compute(input, buffer.size(), output);
    std::string shaStr;
    for (uint8_t val : output) {
      shaStr += std::to_string(int(val));
    }
    return BigInt(shaStr);
  }

  static Message loadFromFile(const std::string &filename) {
    std::fstream file;
    file.open(filename, std::ios::in | std::ios::binary);
    if (file.good()) {
      std::vector<uint8_t>
          buffer((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
      file.close();
      return Message(buffer);
    }
  }
};

#endif //DIGITALSIGNATUREDES_MESSAGE_HPP